/*!
 *@brief    4x4Matrix(Column-Major Matrix)
 *@date     2021-6-23
 */

#pragma once
#include <math.h>
#include "TRSVector.h"

class TRSMatrix
{
public:
    TRSMatrix();
    TRSMatrix(const TRSMatrix& matrix);
    TRSMatrix operator =(const TRSMatrix& matrix);
    ~TRSMatrix();

    TRSVec4& operator[](int col_index);
    TRSVec4 operator[](int col_index) const;

    void makeIdentity();

    void makeTranslate(double x, double y, double z);
    void translate(double x, double y, double z);

    void makeRotate(double angle, double vecx, double vecy, double vecz);
    void rotate(double angle, double vecx, double vecy, double vecz);

    void makeScale(double scale);
    void makeScale(double scalex, double scaley, double scalez);
    void scale(double scale);
    void scale(double scalex, double scaley, double scalez);

    void makeOtho(double l, double r, double b, double t, double n, double f);
    void makePerspective(double l, double r, double b, double t, double n, double f);
    void makePerspective(double fov, double aspect, double n, double f);

    TRSMatrix operator *(const TRSMatrix& matrix) const;

    TRSMatrix& postMultiply(const TRSMatrix& matrix);
    TRSMatrix& preMultiply(const TRSMatrix& matrix);

private:
    TRSVec4 columns[4];         // denote four column
};
