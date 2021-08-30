#include "TRSMatrix.h"
#include <cmath>


TRSMatrix::TRSMatrix()
{
    makeIdentity();
}

TRSMatrix::TRSMatrix(const TRSMatrix& matrix)
{
    for (int i = 0; i < 4; i++) // col
    {
        for (int j = 0; j < 4; j++)// row
        {
            columns[i][j] = matrix[i][j];
        }
    }
}

TRSMatrix TRSMatrix::operator=(const TRSMatrix& matrix)
{
    for (int i = 0; i < 4; i++) // col
    {
        for (int j = 0; j < 4; j++)// row
        {
            columns[i][j] = matrix[i][j];
        }
    }
    return *this;
}

TRSMatrix::~TRSMatrix()
{

}

TRSVec4& TRSMatrix::operator[](int col_index)
{
    return columns[col_index];
}

TRSVec4 TRSMatrix::operator[](int col_index) const
{
    return columns[col_index];
}

void TRSMatrix::makeIdentity()
{
    columns[0][0] = 1.0; columns[0][1] = 0.0; columns[0][2] = 0.0; columns[0][3] = 0.0;
    columns[1][0] = 0.0; columns[1][1] = 1.0; columns[1][2] = 0.0; columns[1][3] = 0.0;
    columns[2][0] = 0.0; columns[2][1] = 0.0; columns[2][2] = 1.0; columns[2][3] = 0.0;
    columns[3][0] = 0.0; columns[3][1] = 0.0; columns[3][2] = 0.0; columns[3][3] = 1.0;
}

void TRSMatrix::makeTranslate(double x, double y, double z)
{
    makeIdentity();
    columns[3][0] = x;
    columns[3][1] = y;
    columns[3][2] = z;
}

void TRSMatrix::translate(double x, double y, double z)
{
    TRSMatrix mat;
    mat.makeTranslate(x, y, z);
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

    columns[0][0] = 1 - 2 * c*c - 2 * d*d;
    columns[0][1] = 2 * b*c + 2 * a*d;
    columns[0][2] = 2 * b*d - 2 * a*c;
    columns[0][3] = 0;

    columns[1][0] = 2 * b*c - 2 * a*d;
    columns[1][1] = 1 - 2 * b*b - 2 * d*d;
    columns[1][2] = 2 * a*b + 2 * c*d;
    columns[1][3] = 0;

    columns[2][0] = 2 * a*c + 2 * b*d;
    columns[2][1] = 2 * c*d - 2 * a*b;
    columns[2][2] = 1 - 2 * b*b - 2 * c*c;
    columns[2][3] = 0;

    columns[3][0] = 0;
    columns[3][1] = 0;
    columns[3][2] = 0;
    columns[3][3] = 1;
}

void TRSMatrix::rotate(double angle, double vecx, double vecy, double vecz)
{
    TRSMatrix mat;
    mat.makeRotate(angle, vecx, vecy, vecz);
    *this = (*this) * mat;
}

void TRSMatrix::makeScale(double scale)
{
    makeScale(scale, scale, scale);
}

void TRSMatrix::makeScale(double scalex, double scaley, double scalez)
{
    makeIdentity();
    columns[0][0] = scalex;
    columns[1][1] = scaley;
    columns[2][2] = scalez;
}

void TRSMatrix::scale(double s)
{
    scale(s, s, s);
}

void TRSMatrix::scale(double scalex, double scaley, double scalez)
{
    TRSMatrix mat;
    mat.makeScale(scalex, scaley, scalez);
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
            result[i][j]
                = columns[0][i] * matrix[j][0]
                + columns[1][i] * matrix[j][1]
                + columns[2][i] * matrix[j][2]
                + columns[3][i] * matrix[j][3];
            
        }
    }
    return result;
}
