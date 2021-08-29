#include "TRSMatrix.h"
#include <cmath>


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

void TRSMatrix::translate(double x, double y, double z)
{
    TRSMatrix mat;
    mat.translate(x, y, z);
    *this = (*this) * mat;
}

void TRSMatrix::makeRotate(double angle, double vecx, double vecy, double vecz)
{
    makeIdentity();
    // v' = p v p*
    // p = [cos(angle/2), sin(angle/2)* u ]
    // u = (ux, uy, uz); normalized
    double length = std::sqrt(vecx*vecx + vecy*vecy + vecz*vecz);
    if (length == 0)
    {
        return;
    }
    vecx = vecx / length;
    vecy = vecy / length;
    vecz = vecz / length;
    double halfAngle = angle / 2;
    double sin = std::sin(halfAngle);
    double a = std::cos(halfAngle);
    double b = sin * vecx;
    double c = sin * vecy;
    double d = sin * vecz;

    element[0][0] = 1 - 2 * c*c - 2 * d*d;
    element[0][1] = 2 * b*c - 2 * a*d;
    element[0][2] = 2 * a*c + 2 * b*d;
    element[0][3] = 0;

    element[1][0] = 2 * b*c + 2 * a*d;
    element[1][1] = 1 - 2 * b*b - 2 * d*d;
    element[1][2] = 2 * c*d - 2 * a*b;
    element[1][3] = 0;

    element[2][0] = 2 * b*d - 2 * a*c;
    element[2][1] = 2 * a*b + 2 * c*d;
    element[2][2] = 1 - 2 * b*b - 2 * c*c;
    element[2][3] = 0;

    element[3][0] = 0; element[3][1] = 0; element[3][2] = 0; element[3][3] = 1;
}

void TRSMatrix::rotate(double angle, double vecx, double vecy, double vecz)
{
    TRSMatrix mat;
    mat.rotate(angle, vecx, vecy, vecz);
    *this = (*this) * mat;
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

void TRSMatrix::scale(double s)
{
    scale(s, s, s);
}

void TRSMatrix::scale(double scalex, double scaley, double scalez)
{
    TRSMatrix mat;
    mat.scale(scalex, scaley, scalez);
    *this = (*this) * mat;
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
