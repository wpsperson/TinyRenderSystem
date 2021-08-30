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

    double distance(const TRSVector<N>& vec);
    double length() const;
    void normalize();
    inline TRSVector<N> cross(const TRSVector<N>& vec);

    double operator[](int i)const;
    double& operator[](int i);

    void set(int i, double value);

private:
    double arr[N];
};

template<>
TRSVector<3> TRSVector<3>::cross(const TRSVector<3>& vec)
{
    TRSVector<3> result;
    result[0] = arr[1] * vec[2] - arr[2] * vec[1];
    result[1] = arr[2] * vec[0] - arr[0] * vec[2];
    result[2] = arr[0] * vec[1] - arr[1] * vec[0];
    return result;
}

template<int N>
void TRSVector<N>::normalize()
{
    double len - length();
    for (int i = 0; i < N; i++)
        arr[i] /= len;
}

template<int N>
double TRSVector<N>::length() const
{
    double sum = 0;
    for (int i = 0; i < N; i++)
        sum += arr[i] * arr[i];
    return std::sqrt(sum);
}

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
double TRSVector<N>::distance(const TRSVector<N>& vec)
{
    TRSVector<N> substract = vec - *this;
    return substract.length();
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

