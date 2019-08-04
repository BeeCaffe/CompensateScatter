#ifndef COMPENSATESCATTER_SCATTER_H
#define COMPENSATESCATTER_SCATTER_H

#include <cmath>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Eigen/Core"
#include "../include/utils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include "Eigen/Dense"

/**
 * This class is an implement version of paper "Compensating Indirect Scattering for immersive and Semi-Immersive
 * Projection Display", which is used to compensate the projector-camera system in L-shape screen.This method is a
 * tradition method for photometric compensation and need to understand the geometric information of projection
 * environment,and using iterator method to compensate images eventually.The detail inner-principle of this
 * program you'd better to refer the paper,which I have provided in the "resource/paper" directory.
 */

#define PI (3.14159265358979323846)

class Scatter{

public:
    //default constructor
    Scatter();

    //desturctor
    ~Scatter()= default;

    //two overload function of "computeF()",which is used to calculate the energy function of light I
    void onePatchF();
    void doublePatchF();

    //slove the linear function of the radiosity B.
    Eigen::MatrixXf radiosity();

    //calculates all patch's scatter of light.
    cv::Mat computeScatter(const cv::Mat& I);

    //calculates all patch's compensated light, called "Ic".
    cv::Mat compensateI(const cv::Mat &R,const cv::Mat &S);

    //compensate single image and return it.
    cv::Mat compensateImg(const cv::Mat& R);

    //the compensating end conditional.
    bool terminate(const cv::Mat I,const cv::Mat I_next);

    //compensates all images in the image file root and save them to the save root.
    void compensateImgs();
private:
    //the configure file path
    std::string confPath="conf/conf.txt";

    //The width of projection image
    size_t imWidth;

    //The height of projection image
    size_t imHeight;

    //The total width of L-shape image plane,which is a part of screen with image.
    float_t screenWidth;

    //The height of L-shape image plane.
    float_t screenHeight;

    //The height of middle line of image plane
    float_t screenMidHeight;

    //The distance between projector and screen center.
    float_t centerDist;

    //The patch size
    int patchSize;

    //the horizontal patch
    int u;
    //the vertical patch
    int v;

    //the real length of patch in the screen
    float_t detaX;
    //the real length of patch in the screen
    float_t detaYEd;
    //the real length of patch in the screen
    float_t detaYMd;

    Eigen::MatrixXf Fi;
    Eigen::MatrixXf Fij;

    std::string imRoot;
    std::string saveRoot;

    float f=1;

};
#endif //COMPENSATESCATTER_SCATTER_H
