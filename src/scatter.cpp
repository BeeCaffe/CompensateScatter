#include "../include/scatter.h"

Scatter::Scatter():
screenWidth(.47),
screenHeight(.18),
screenMidHeight(.25),
centerDist(.55){
}

Scatter::Scatter(
        const float_t wd,
        const float_t ht,
        const float_t midHt,
        const float_t ctDis):
        screenWidth(wd),
        screenHeight(ht),
        screenMidHeight(midHt),
        centerDist(ctDis){
}