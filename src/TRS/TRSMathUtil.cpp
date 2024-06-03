#include "TRS/TRSMathUtil.h"
#include "TRS/TRSDefMath.h"


float toRadian(float degree)
{
    return static_cast<float>(degree * kDegreeToRad);
}

float toDegree(float radians)
{
    return static_cast<float>(radians * kRadToDegree);
}
