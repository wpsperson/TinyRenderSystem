/*!
 *@brief    Define Vector
 *@date     2021-8-30
 */

#pragma once
#include "Core\TRSExport.h"
#include <cmath>

template<int N>
class TRSVector
{
public:
    TRSVector();
    TRSVector(const TRSVector<N>& vec);
    TRSVector<N> operator=(const TRSVector<N>& vec);
    inline TRSVector<N>(float x, float y);
    inline TRSVector<N>(float x, float y, float z);
    inline TRSVector<N>(float x, float y, float z, float w);
    ~TRSVector();
    TRSVector<N> operator -() const;
    bool operator == (const TRSVector<N>& vec) const;
    bool operator != (const TRSVector<N>& vec) const;
    TRSVector<N> operator + (const TRSVector<N>& vec) const;
    TRSVector<N> operator - (const TRSVector<N>& vec) const;
    void operator += (const TRSVector<N>& vec);
    void operator -= (const TRSVector<N>& vec);
    TRSVector<N> operator * (float value) const;

    float distance(const TRSVector<N>& vec);
    float length() const;
    void normalize();
    inline TRSVector<N> cross(const TRSVector<N>& vec) const;
    inline float dot(const TRSVector<N>& vec) const;

    const float& operator[](int i)const;
    float& operator[](int i);
    float* pointer();
    void set(int i, float value);

private:
    float arr[N];
};


template<int N>
bool TRSVector<N>::operator != (const TRSVector<N>& vec) const
{
    return !((*this) == vec);
}

template<int N>
bool TRSVector<N>::operator==(const TRSVector<N>& vec) const
{
    for (int i=0; i<N; i++)
    {
        if (arr[i] != vec[i])
            return false;
    }
    return true;
}

template<int N>
TRSVector<N> TRSVector<N>::operator -() const
{
    TRSVector<N> result = (*this) * (-1);
    return result;
}

template<int N>
float TRSVector<N>::dot(const TRSVector<N>& vec) const
{
    float sum = 0.0f;
    for (int i=0; i<N; i++)
    {
        sum += arr[i] * vec[i];
    }
    return sum;
}

template<int N>
float* TRSVector<N>::pointer()
{
    return arr;
}

template<>
TRSVector<3> TRSVector<3>::cross(const TRSVector<3>& vec) const
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
    float len = length();
    for (int i = 0; i < N; i++)
        arr[i] /= len;
}

template<int N>
float TRSVector<N>::length() const
{
    float sum = 0;
    for (int i = 0; i < N; i++)
        sum += arr[i] * arr[i];
    return std::sqrt(sum);
}

template <>
TRSVector<2>::TRSVector(float x, float y)
{
    arr[0] = x;
    arr[1] = y;
}
template <>
TRSVector<3>::TRSVector(float x, float y, float z)
{
    arr[0] = x;
    arr[1] = y;
    arr[2] = z;
}
template <>
TRSVector<4>::TRSVector(float x, float y, float z, float w)
{
    arr[0] = x;
    arr[1] = y;
    arr[2] = z;
    arr[3] = w;
}

template<int N>
float TRSVector<N>::distance(const TRSVector<N>& vec)
{
    TRSVector<N> substract = vec - *this;
    return substract.length();
}

template<int N>
TRSVector<N> TRSVector<N>::operator*(float value) const
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
void TRSVector<N>::operator+=(const TRSVector<N>& vec)
{
    *this = *this + vec;
}

template<int N>
void TRSVector<N>::operator-=(const TRSVector<N>& vec)
{
    *this = *this - vec;
}

template<int N>
void TRSVector<N>::set(int i, float value)
{
    arr[i] = value;
}

template<int N>
const float& TRSVector<N>::operator[](int i) const
{
    return arr[i];
}

template<int N>
float& TRSVector<N>::operator[](int i)
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

typedef TRSVector<2> TRSVec2;
typedef TRSVector<3> TRSVec3;
typedef TRSVector<3> TRSPoint;
typedef TRSVector<4> TRSVec4;

const TRSVec3 G_ORIGIN(0.0f, 0.0f, 0.0f);
const TRSVec3 G_XDIR(1.0f, 0.0f, 0.0f);
const TRSVec3 G_YDIR(0.0f, 1.0f, 0.0f);
const TRSVec3 G_ZDIR(0.0f, 0.0f, 1.0f);
