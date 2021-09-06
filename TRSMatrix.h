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

    void makeTranslate(float x, float y, float z);
    void translate(float x, float y, float z);

    void makeRotate(float angle, float vecx, float vecy, float vecz);
    void rotate(float angle, float vecx, float vecy, float vecz);

    void makeScale(float scale);
    void makeScale(float scalex, float scaley, float scalez);
    void scale(float scale);
    void scale(float scalex, float scaley, float scalez);

    void makeOtho(float l, float r, float b, float t, float n, float f);
    void makePerspective(float l, float r, float b, float t, float n, float f);
    void makePerspective(float fov, float aspect, float n, float f);

    TRSMatrix operator *(const TRSMatrix& matrix) const;

    TRSMatrix& postMultiply(const TRSMatrix& matrix);
    TRSMatrix& preMultiply(const TRSMatrix& matrix);

private:
    TRSVec4 columns[4];         // denote four column
};
