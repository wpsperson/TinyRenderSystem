/*!
 *@brief    
 *@date     2021-6-23
 */

#pragma once
#include <math.h>

class TRSMatrix
{
public:
    TRSMatrix();
    ~TRSMatrix();

private:
    double element[4][4];
};
