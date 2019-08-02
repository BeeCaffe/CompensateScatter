//
// Created by caffe on 19-8-2.
//
#include "../include/utils.h"
#include "../include/scatter.h"

void TestConstructor(){

    Scatter scatter=Scatter();
    printf("DONE!");
}

void TestFi(){
    Scatter scatter=Scatter();
    scatter.onePatchF();
    printf("DONE!");
}

void TestFij(){
    Scatter scatter=Scatter();
    scatter.doublePatchF();
    printf("DONE!");
}

void TestComputeScatter(){
    Scatter scatter=Scatter();
    cv::Mat img=cv::imread("resources/images/00061.jpg");
    img.convertTo(img,CV_32FC3);
    cv::Mat S=scatter.computeScatter(img);
    std::cout<<S<<std::endl;
    printf("DONE!");
}

void TestCompensateI(){
    Scatter scatter=Scatter();
    cv::Mat img=cv::imread("resources/images/00061.jpg");
    img.convertTo(img,CV_32FC3);
    cv::Mat S=scatter.computeScatter(img);
    cv::Mat I=scatter.compensateI(img,S);
    std::cout<<I<<std::endl;
    img.release();
    S.release();
    I.release();
    printf("DONE!");
}
int main(int argc, char** argv){
//    TestConstructor();
//    TestFi();
    TestFij();
//    TestComputeScatter();
//    TestCompensateI();
    return 0;
}
