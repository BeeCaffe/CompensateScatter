#ifndef COMPENSATESCATTER_UTILS_H
#define COMPENSATESCATTER_UTILS_H
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <unistd.h>

void readConf(const std::string fPath, std::map<std::string,std::string> &confMap);
void process(const int now, const int total , time_t statTm, time_t endTm);
#endif //COMPENSATESCATTER_UTILS_H
