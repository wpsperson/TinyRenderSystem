#include "Math\TRSMathUtil.h"
#define _USE_MATH_DEFINES
#include <math.h>


float toRadian(float degree)
{
    return static_cast<float>(degree * M_PI / 180.0);
}

float toDegree(float radians)
{
    return static_cast<float>(radians * 180.0 / M_PI);
}
