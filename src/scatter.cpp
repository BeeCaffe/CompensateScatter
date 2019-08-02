#include "../include/scatter.h"

Scatter::Scatter(){
    std::map<std::string,std::string> confMap;
    readConf(confPath,confMap);
    std::map<std::string,std::string>::iterator iter;
    iter = confMap.find("ImageWidth");
    imWidth = atoi(iter->second.c_str());
    iter = confMap.find("ImageHeight");
    imHeight = atoi(iter->second.c_str());
    iter = confMap.find("ScreenWidth");
    screenWidth = atof(iter->second.c_str());
    iter = confMap.find("ScreenHeight");
    screenHeight = atof(iter->second.c_str());
    iter = confMap.find("ScreenMiddleHeight");
    screenMidHeight = atof(iter->second.c_str());
    iter = confMap.find("CenterDistance");
    centerDist = atof(iter->second.c_str());
    iter = confMap.find("PatchSize");
    patchSize = atoi(iter->second.c_str());
    iter = confMap.find("ImagesRoot");
    imRoot = iter->second.c_str();
    iter = confMap.find("ImagesSaveRoot");
    saveRoot = iter->second.c_str();
    u=imWidth/patchSize;
    v=imHeight/patchSize;
    detaX = screenWidth/u;
    detaYEd = screenHeight/v;
    detaYMd = screenMidHeight/v;
    if(!access(saveRoot.c_str(),0)){
        mkdir(saveRoot.c_str(),0775);
    }
}

/**
 * @brief : This function is used to calculate the energy function of light I, which formulation is "F = f*dA*cos(a)/(r^2*pi)".
 *          we will calculate all patch's energy function and save them in a matrix, more detail information about this function
 *          please refer to the paper.
 * @return F: the energy function of light I,with a shape of (u,v),where v=height/patchSize and u=width/patchSize.
 */
void Scatter::onePatchF(){
    Fi=Eigen::MatrixXf(v,u);
    for(size_t i=0;i<u*v;i++){
        float xi=(i%u)*detaX+detaX/2.;
        float yi=screenHeight-(i/u)*detaYEd-detaYEd/2.;
        float d = sqrt(-(1.1*(screenWidth/2.-xi)*0.5253-(screenWidth/2.-xi)*(screenWidth/2.-xi)-pow(.55,2)));
        float ri=sqrt(d*d+yi*yi);
        float alpha_i=PI/2-atan(d/yi);
        float dA=detaX*(detaYMd+detaYEd)/2.;
        float ftmp=dA*cos(alpha_i)/(ri*ri*PI);
        int col=i%u;
        int row=i/u;
        Fi(row,col)=f*ftmp;
    }
//    std::cout<<Fi<<std::endl;
}

/**
 * @brief : This function is used to calculate the energy function of light I, which formulation is "F = f*dA*cos(a)/(r^2*pi)".
 *          we will calculate all patch's energy function and save them in a matrix, and there are some difference of onePatchF()
 *          that it computes each patch's F, more detail information about this function please refer to the paper.
 * @return F: the energy function of light I,with a shape of (x,y), where x= uv, y = uv
 */
void Scatter::doublePatchF() {
    Fij=Eigen::MatrixXf(u*v,u*v);
    for(size_t i=0;i<u*v;i++){
        for(size_t j=0;j<u*v;j++){
            if(i==j){
                Fij(i,j)=0.;
            } else{
                if((i/u<u/2 && j/u>u/2) || (i/u>u/2 && j/u<u/2)){
                    float_t xi=imWidth/2.-(i%u)*detaX+detaX/2.;
                    float_t xj=imWidth/2.+(j%u)*detaX+detaX/2.;
                    float_t rij=sqrt(xi*xi+xj*xj);
                    float_t alpha_i=PI/2.-atan(xi/xj);
                    float_t alpha_j=PI/2.-atan(xj/xi);
                    float_t dAi=cos(atan(xi/xj))*detaX*(detaYEd+detaYMd)/2.;
                    float_t ftmp=dAi*cos(alpha_i)*cos(alpha_j)/(rij*rij*PI);
                    Fij(i,j)=f*ftmp;
                } else{
                    float_t xi=0;
                }
            }
        }
    }
    std::cout<<Fij<<std::endl;
}

/**
 * @brief : this function is used to compute the Scatter matrix which is represent as 'S' in the paper.In fact, the scatter matrix
 *          is as the shape of image.
 * @param I : the initial image.
 * @return S : the scatter matrix and saved in a cv::Mat.
 */
cv::Mat Scatter::computeScatter(const cv::Mat& I){
    onePatchF();
    doublePatchF();
    cv::Mat S=cv::Mat(imHeight,imWidth,CV_32FC3,cv::Scalar(0.));
    time_t staTm=time((time_t*)NULL);
    printf("Computing S : \n");
    for(size_t i=0;i<u*v;i++){
        size_t row=i/u,col=i%u;
        cv::Mat patch=I(cv::Range(row*patchSize,(row+1)*patchSize),cv::Range(col*patchSize,(col+1)*patchSize));
        for(size_t j=0;j<u*v;j++){
            S(cv::Range(row*patchSize,(row+1)*patchSize),cv::Range(col*patchSize,(col+1)*patchSize))+=patch*Fi(i)*Fij(j,i);
        }
        time_t endTm=time((time_t*)NULL);
        process(i,u*v,staTm,endTm);
    }
    return S;
}

/**
 * @brief : this function is used to compensate the image using the scatter matrix S,and the formulation is I=(R-S)/F
 * @param R : the initial image.
 * @param S : the scatter matrix.
 * @return I : the compensated matrix.
 */
cv::Mat Scatter::compensateI(const cv::Mat &R,const cv::Mat &S){
    cv::Mat I=cv::Mat(imHeight,imWidth,CV_32FC3,cv::Scalar(0));//save the compensated image
    size_t cols=R.cols;
    size_t rows=R.rows;
    time_t staTm=time((time_t*)NULL);
    printf("Compensating : \n");
    I=(R-S);
    for(size_t i=0;i<u*v;i++){
        size_t row=i/u,col=i%u;
        I(cv::Range(row*patchSize,(row+1)*patchSize),cv::Range(col*patchSize,(col+1)*patchSize))/=Fi(i);
        time_t endTm=time((time_t*)NULL);
        process(i,u*v,staTm,endTm);
    }
    return I;
}

/**
 * @brief : compensating single image and convert it format to CV_8UC3, and return it.
 * @param R : the initial image which type of CV_32FC3
 * @return I_next: the compensated image.
 */
cv::Mat Scatter::compensateImg(const cv::Mat& R){

    cv::Mat S=computeScatter(R);
    cv::Mat I=compensateI(R,S);
//    cv::Mat S_next=computeScatter(I);
//    cv::Mat I_next=compensateI(R,S_next);
//    size_t count=2;
//    while(!terminate(I,I_next) && count<=7){
//        I=I_next;
//        S_next=computeScatter(I);
//        I_next=compensateI(R,S_next);
//        count++;
//    }
//    std::cout<<I_next<<std::endl;
//    I_next.convertTo(I_next,CV_8UC3);
//    return I_next;
    I.convertTo(I,CV_8UC3);
    return I;
}

/**
 * @brief : judge where the compensation loop should be stop, if the difference of two consecutive compensated images
 *          are not more than on gray scale unit, the loop should stop.
 * @param I : the i times compensated image.
 * @param I_next : the i+1 times compensated image.
 * @return true or flase.
 */
bool Scatter::terminate(const cv::Mat I,const cv::Mat I_next){

    cv::Mat detaI=I_next-I;

    size_t cols=detaI.cols;

    size_t rows=detaI.rows;

    double ave=0;

    for(size_t i=0;i<rows;i++){

        for(size_t j=0;j<cols;j++){

            ave+=(detaI.ptr<cv::Vec3f>(i)[j][0]+detaI.ptr<cv::Vec3f>(i)[j][1]+detaI.ptr<cv::Vec3f>(i)[j][2])/cols*rows*3;
        }
    }
    printf("average = %lf\n",ave);
    if(ave<1) return true;
    else return false;
}

/**
 * @brief : cmpensate all images in the image save root and save them to a new directory.
 */
void Scatter::compensateImgs(){
    std::vector<cv::String> imList;
    cv::glob(imRoot,imList);
    for(auto filename:imList){
        cv::Mat img = cv::imread(filename,CV_LOAD_IMAGE_UNCHANGED);
        img.convertTo(img,CV_32FC3);
        cv::Mat n_img=compensateImg(img);
        std::string savename = saveRoot+filename.substr(
                filename.find_last_of('/'),
                filename.find_last_of('.')
                );
        cv::imwrite(savename,n_img);
        img.release();
        n_img.release();
        printf("Compensated an image!\n");
    }
    printf("DONE!\n");
}


