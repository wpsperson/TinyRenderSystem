/*!
 *@brief    4x4Matrix(Column-Major Matrix)
 *@date     2021-6-23
 */

#pragma once
#include <math.h>
#include "Core\TRSExport.h"
#include "Math\TRSVector.h"

class TRS_EXPORT TRSMatrix
{
public:
    TRSMatrix();
    TRSMatrix(const TRSMatrix& matrix);
    TRSMatrix operator =(const TRSMatrix& matrix);
    ~TRSMatrix();

    TRSVec4& operator[](int col_index);
    const TRSVec4& operator[](int col_index) const;

    void makeIdentity();

    void makeTranslate(float x, float y, float z);
    void makeTranslate(const TRSVec3& trans);
    void translate(float x, float y, float z);
    void translate(const TRSVec3& trans);

    void makeRotate(float angle, float vecx, float vecy, float vecz);
    void makeRotate(float angle, const TRSVec3& axis);
    void rotate(float angle, float vecx, float vecy, float vecz);
    void rotate(float angle, const TRSVec3& axis);

    void makeScale(float scale);
    void makeScale(float scalex, float scaley, float scalez);
    void scale(float scale);
    void scale(float scalex, float scaley, float scalez);

    void makeOtho(float l, float r, float b, float t, float n, float f);
    void makePerspective(float l, float r, float b, float t, float n, float f);
    void makePerspective(float fov, float aspect, float n, float f);

    void makeLookat(const TRSVec3& eye, const TRSVec3& front, const TRSVec3& up);

    TRSMatrix operator *(const TRSMatrix& matrix) const;
    TRSVec4 operator *(const TRSVec4& vec) const;
    TRSVec3 operator *(const TRSVec3& vec) const;

    TRSMatrix& postMultiply(const TRSMatrix& matrix);
    TRSMatrix& preMultiply(const TRSMatrix& matrix);

    float determinant() const;

    TRSMatrix getInverse() const;

private:
    TRSVec4 columns[4];         // denote four column
};
