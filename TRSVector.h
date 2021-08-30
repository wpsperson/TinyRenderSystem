/*!
 *@brief    Define Vector
 *@date     2021-8-30
 */

#pragma once
#include "TRSExport.h"

template<int N>
class TRS_EXPORT TRSVector
{
public:
    TRSVector();
    TRSVector(const TRSVector<N>& vec);
    TRSVector<N> operator=(const TRSVector<N>& vec);
    inline TRSVector<N>(double x, double y);
    inline TRSVector<N>(double x, double y, double z);
    inline TRSVector<N>(double x, double y, double z, double w);
    ~TRSVector();
    TRSVector<N> operator + (const TRSVector<N>& vec) const;
    TRSVector<N> operator - (const TRSVector<N>& vec) const;
    TRSVector<N> operator * (double value) const;
    double distSquare(const TRSVector<N>& vec);
    double dist(const TRSVector<N>& vec);

    double operator[](int i)const;
    double& operator[](int i);

    void set(int i, double value);

private:
    double arr[N];
};

template <>
TRSVector<2>::TRSVector(double x, double y)
{
    arr[0] = x;
    arr[1] = y;
}
template <>
TRSVector<3>::TRSVector(double x, double y, double z)
{
    arr[0] = x;
    arr[1] = y;
    arr[2] = z;
}
template <>
TRSVector<4>::TRSVector(double x, double y, double z, double w)
{
    arr[0] = x;
    arr[1] = y;
    arr[2] = z;
    arr[3] = w;
}

template<int N>
double TRSVector<N>::dist(const TRSVector<N>& vec)
{
    return std::sqrt(distSquare(vec));
}

template<int N>
double TRSVector<N>::distSquare(const TRSVector<N>& vec)
{
    double sum = 0;
    for (int i = 0; i < N; i++)
        sum += (this->arr[i] - vec[i])*(this->arr[i] - vec[i]);
    return sum;
}

template<int N>
TRSVector<N> TRSVector<N>::operator*(double value) const
{
    TRSVector<N> result;
    for (int i = 0; i < N; i++)
        result.set(i, this->arr[i] * value);
    return result;
}


template<int N>
TRSVector<N> TRSVector<N>::operator-(const TRSVector<N>& vec) const
{
    TRSVector<N> result = *this + vec*(-1);
    return result;
}

template<int N>
TRSVector<N> TRSVector<N>::operator+(const TRSVector<N>& vec) const
{
    TRSVector<N> result;
    for (int i = 0; i < N; i++)
        result.set(i, this->arr[i] + vec[i]);
    return result;
}

template<int N>
void TRSVector<N>::set(int i, double value)
{
    arr[i] = value;
}

template<int N>
double TRSVector<N>::operator[](int i) const
{
    return arr[i];
}

template<int N>
double& TRSVector<N>::operator[](int i)
{
    return arr[i];
}

template<int N>
TRSVector<N>::~TRSVector()
{

}

template<int N>
TRSVector<N>::TRSVector()
{
    for (int i = 0; i < N; i++)
        arr[i] = 0;
}

template<int N>
TRSVector<N>::TRSVector(const TRSVector<N>& vec)
{
    for (int i = 0; i < N; i++)
        arr[i] = vec[i];
}

template<int N>
TRSVector<N> TRSVector<N>::operator=(const TRSVector<N>& vec)
{
    for (int i = 0; i < N; i++)
        arr[i] = vec[i];
    return *this;
}

typedef TRSVector<3> TRSVec3;
typedef TRSVector<4> TRSVec4;

