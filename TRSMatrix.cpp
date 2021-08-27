#include "TRSMatrix.h"



TRSMatrix::TRSMatrix()
{
    makeIdentity();
}

TRSMatrix::~TRSMatrix()
{

}

void TRSMatrix::makeIdentity()
{
    element[0][0] = 0.0; element[0][1] = 0.0; element[0][2] = 0.0; element[0][3] = 0.0;
    element[1][0] = 0.0; element[1][1] = 0.0; element[1][2] = 0.0; element[1][3] = 0.0;
    element[2][0] = 0.0; element[2][1] = 0.0; element[2][2] = 0.0; element[2][3] = 0.0;
    element[3][0] = 0.0; element[3][1] = 0.0; element[3][2] = 0.0; element[3][3] = 0.0;
}
