/*!
 *@brief    4x4Matrix(Column-Major Matrix)
 *@date     2021-6-23
 */

#pragma once
#include <math.h>

class TRSMatrix
{
public:
    TRSMatrix();
    TRSMatrix(const TRSMatrix& matrix);
    TRSMatrix operator =(const TRSMatrix& matrix);
    ~TRSMatrix();

    void setEle(int row, int col, double value);
    double ele(int row, int col) const;

    void makeIdentity();

    void makeTranslate(double x, double y, double z);
    void translate(double x, double y, double z);

    void makeRotate(double angle, double vecx, double vecy, double vecz);
    void rotate(double angle, double vecx, double vecy, double vecz);

    void makeScale(double scale);
    void makeScale(double scalex, double scaley, double scalez);
    void scale(double scale);
    void scale(double scalex, double scaley, double scalez);

    TRSMatrix operator *(const TRSMatrix& matrix) const;

    TRSMatrix& postMultiply(const TRSMatrix& matrix);
    TRSMatrix& preMultiply(const TRSMatrix& matrix);

private:
    double element[4][4];
};
