/**
 * Created by caffe on 19-8-2.
 * */

#include "../include/utils.h"

/**
 * @brief : this function is used to read the configure file, and all value would be returned as string.
 * @param fPath : the confiure file path
 * @param confMap : the configure map, you can get the "value" by "key", and all type would be default set to string.
 */
void readConf(const std::string fPath, std::map<std::string,std::string> &confMap){

    std::ifstream confFile;

    std::string strLine;

    confFile.open(fPath);

    if(confFile.is_open()){

        while(!confFile.eof()){

            std::getline(confFile,strLine);

            if(strLine.find('#')==0){
                continue;
            }

            size_t pos = strLine.find('=');

            std::string key = strLine.substr(0,pos);

            std::string value = strLine.substr(pos+1);

            confMap.insert(std::pair<std::string,std::string>(key,value));

        }
    } else{

        perror("readConf error : the configure file open failure!");
    }
}

/**
 * @brief : this function is used to show the process of program.
 * @param now : the number of now.
 * @param total : the number of total.
 * @param statTm : start time.
 * @param endTm : end time
 */
void process(const int now, const int total , time_t statTm, time_t endTm){
    int range = 80;
    time_t detaTm = endTm - statTm;
    time_t h = detaTm/3600;
    time_t m = (detaTm-h*3600)/60;
    time_t s = detaTm%60;
    double pcent = ((double)now/total)*100;
    printf("Number:%d/%d Rate:%2.4lf%% Time:%d:%d:%d [",now,total,pcent,h,m,s);
    for(size_t i=0;i<((double)now/total)*range;i++){
        printf("#");
    }
    for(size_t j=((double)now/total)*range;j<range;j++ ){
        printf(" ");
    }
    printf("]");
    fflush(stdout);
    std::cout<<'\r';
}

