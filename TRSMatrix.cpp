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

void TRSMatrix::makeTranslate(float x, float y, float z)
{
    makeIdentity();
    columns[3][0] = x;
    columns[3][1] = y;
    columns[3][2] = z;
}

void TRSMatrix::translate(float x, float y, float z)
{
    TRSMatrix mat;
    mat.makeTranslate(x, y, z);
    *this = (*this) * mat;
}

void TRSMatrix::makeRotate(float angle, float vecx, float vecy, float vecz)
{
    makeIdentity();
    // v' = p v p*
    // p = [cos(angle/2), sin(angle/2)* u ]
    // u = (ux, uy, uz); normalized
    float length = std::sqrt(vecx*vecx + vecy*vecy + vecz*vecz);
    if (length == 0)
    {
        return;
    }
    vecx = vecx / length;
    vecy = vecy / length;
    vecz = vecz / length;
    float halfAngle = angle / 2;
    float sin = std::sin(halfAngle);
    float a = std::cos(halfAngle);
    float b = sin * vecx;
    float c = sin * vecy;
    float d = sin * vecz;

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

void TRSMatrix::rotate(float angle, float vecx, float vecy, float vecz)
{
    TRSMatrix mat;
    mat.makeRotate(angle, vecx, vecy, vecz);
    *this = (*this) * mat;
}

void TRSMatrix::makeScale(float scale)
{
    makeScale(scale, scale, scale);
}

void TRSMatrix::makeScale(float scalex, float scaley, float scalez)
{
    makeIdentity();
    columns[0][0] = scalex;
    columns[1][1] = scaley;
    columns[2][2] = scalez;
}

void TRSMatrix::scale(float s)
{
    scale(s, s, s);
}

void TRSMatrix::scale(float scalex, float scaley, float scalez)
{
    TRSMatrix mat;
    mat.makeScale(scalex, scaley, scalez);
    *this = (*this) * mat;
}

/*
clip volume center coordinate is [ (l+r)/2, (t+b)/2, -(f+n)/2 ]
translate matrix: we first move clip volume to make volume center to coincide with view origin. 
scale matrix:    scale the clip volume to make it dimension is [-1,1]. then reverse z axis
|2/(r-l), 0,       0,       0| |1, 0, 0, -(r+l)/2|
|0,       2/(t-b), 0,       0| |0, 1, 0, -(t+b)/2|
|0,       0,      -2/(f-n), 0| |0, 0, 1,  (f+n)/2|
|0,       0,       0,       1| |0, 0, 0,        1|
calculate above matrix we get
|2/(r-l), 0,       0,       -(r+l)/(r-l)|
|0,       2/(t-b), 0,       -(t+b)/(t-b)|
|0,       0,      -2/(f-n), -(f+n)/(f-n)|
|0,       0,       0,                  1|
*/

void TRSMatrix::makeOtho(float l, float r, float b, float t, float n, float f)
{
    columns[0][0] = 2.0f / (r - l);
    columns[0][1] = 0;
    columns[0][2] = 0;
    columns[0][3] = 0;

    columns[1][0] = 0;
    columns[1][1] = 2.0f / (t - b);
    columns[1][2] = 0;
    columns[1][3] = 0;

    columns[2][0] = 0;
    columns[2][1] = 0;
    columns[2][2] = -2.0f / (f - n);
    columns[2][3] = 0;

    columns[3][0] = -(r + l) / (r - l);
    columns[3][1] = -(t + b) / (t - b);
    columns[3][2] = -(f + n) / (f - n);
    columns[3][3] = 1;
}

/*
x' = x*n/(-z);
y' = y*n/(-z);
z' = (Az+B)/(-z); 
we make it meet for z =-n and z = -f; => A = (n+f) & B = fn
|2/(r-l), 0,       0,       0| |1, 0, 0, -(r+l)/2| |n, 0,  0, 0|
|0,       2/(t-b), 0,       0| |0, 1, 0, -(t+b)/2| |0, n,  0, 0|
|0,       0,      -2/(f-n), 0| |0, 0, 1,  (f+n)/2| |0, 0,  A, B|
|0,       0,       0,       1| |0, 0, 0,        1| |0, 0, -1, 0|
calculate above matrix we get
|2n/(r-l), 0,       (r+l)/(r-l),           0|
|0,       2n/(t-b), (t+b)/(t-b),           0|
|0,       0,        -(f+n)/(f-n), -2fn/(f-n)|
|0,       0,        -1,                    0|
*/


void TRSMatrix::makePerspective(float l, float r, float b, float t, float n, float f)
{
    columns[0][0] = 2.0f * n / (r - l);
    columns[0][1] = 0;
    columns[0][2] = 0;
    columns[0][3] = 0;

    columns[1][0] = 0;
    columns[1][1] = 2.0f * n / (t - b);
    columns[1][2] = 0;
    columns[1][3] = 0;

    columns[2][0] = (r + l) / (r - l);
    columns[2][1] = (t + b) / (t - b);
    columns[2][2] = -(f + n) / (f - n);
    columns[2][3] = -1;

    columns[3][0] = 0;
    columns[3][1] = 0;
    columns[3][2] = -2 * f * n / (f - n);
    columns[3][3] = 0;
}

void TRSMatrix::makePerspective(float fov, float aspect, float n, float f)
{
    float t = std::tan(fov / 2)*n;
    float b = -t;
    float r = t * aspect;
    float l = -r;
    makePerspective(l, r, b, t, n, f);
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
