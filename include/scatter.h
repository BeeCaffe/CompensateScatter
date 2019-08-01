#ifndef COMPENSATESCATTER_SCATTER_H
#define COMPENSATESCATTER_SCATTER_H

#include <cmath>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Eigen/Core"
/**
 * This class is an implement version of paper "Compensating Indirect Scattering for immersive and Semi-Immersive
 * Projection Display", which is used to compensate the projector-camera system in L-shape screen.This method is a
 * tradition method for photometric compensation and need to understand the geometric information of projection
 * environment,and using iterator method to compensate images eventually.The detail inner-principle of this
 * program you'd better to refer the paper,which I have provided in the "resource/paper" directory.
 */
class Scatter{
public:
    Scatter();
    Scatter(const float_t wd,
            const float_t ht,
            const float_t midHt,
            const float_t ctDis);

    cv::Mat
    ~Scatter()= default;
private:
    //The total width of L-shape image plane,which is a part of screen with image.
    const float_t screenWidth;
    //The height of L-shape image plane.
    const float_t screenHeight;
    //The height of middle line of image plane
    const float_t screenMidHeight;
    //The distance between projector and screen center.
    const float_t centerDist;

};
#endif //COMPENSATESCATTER_SCATTER_H
