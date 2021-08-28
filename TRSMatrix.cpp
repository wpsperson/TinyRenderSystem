#include "TRSMatrix.h"



TRSMatrix::TRSMatrix()
{
    makeIdentity();
}

TRSMatrix::TRSMatrix(const TRSMatrix& matrix)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            element[i][j] = matrix.ele(i, j);
        }
    }
}

TRSMatrix TRSMatrix::operator=(const TRSMatrix& matrix)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            element[i][j] = matrix.ele(i, j);
        }
    }
    return *this;
}

TRSMatrix::~TRSMatrix()
{

}

void TRSMatrix::setEle(int row, int col, double value)
{
    element[row][col] = value;
}

double TRSMatrix::ele(int row, int col) const
{
    return element[row][col];
}

void TRSMatrix::makeIdentity()
{
    element[0][0] = 1.0; element[0][1] = 0.0; element[0][2] = 0.0; element[0][3] = 0.0;
    element[1][0] = 0.0; element[1][1] = 1.0; element[1][2] = 0.0; element[1][3] = 0.0;
    element[2][0] = 0.0; element[2][1] = 0.0; element[2][2] = 1.0; element[2][3] = 0.0;
    element[3][0] = 0.0; element[3][1] = 0.0; element[3][2] = 0.0; element[3][3] = 1.0;
}

void TRSMatrix::makeTranslate(double x, double y, double z)
{
    makeIdentity();
    element[3][0] = x;
    element[3][1] = y;
    element[3][2] = z;
}

void TRSMatrix::makeRotate(double angle, double vecx, double vecy, double vecz)
{
    makeIdentity();
    // to do
}

void TRSMatrix::makeScale(double scale)
{
    makeScale(scale, scale, scale);
}

void TRSMatrix::makeScale(double scalex, double scaley, double scalez)
{
    makeIdentity();
    element[0][0] = scalex;
    element[1][1] = scaley;
    element[2][2] = scalez;
}

TRSMatrix& TRSMatrix::postMultiply(const TRSMatrix& matrix)
{
    *this = (*this) * matrix;
    return *this;
}

TRSMatrix& TRSMatrix::preMultiply(const TRSMatrix& matrix)
{
    *this = matrix * (*this);
    return *this;
}

TRSMatrix TRSMatrix::operator*(const TRSMatrix& matrix) const
{
    TRSMatrix result;
    for (int i=0; i<4;i++)
    {
        for (int j=0; j<4; j++)
        {
            double sum
                = element[i][0] * matrix.ele(0, j)
                + element[i][1] * matrix.ele(1, j)
                + element[i][2] * matrix.ele(2, j)
                + element[i][3] * matrix.ele(3, j);
            result.setEle(i, j, sum);
        }
    }
    return result;
}
