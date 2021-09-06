#include "TRSMathUtil.h"
#define _USE_MATH_DEFINES
#include <math.h>


float toRadian(float degree)
{
    return degree * M_PI / 180.0f;
}

float toDegree(float radians)
{
    return radians * 180.0f/ M_PI;
}
