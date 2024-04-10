#pragma once

#include "Geometry/Geometry.h"

namespace geom
{
class EXPORT_GEOM GPoint2d : public Geometry
{
public:
	GPoint2d();

	GPoint2d(real ptx, real pty);

	~GPoint2d();

	void setX(real ptx);

	void setY(real pty);

	void setValue(real ptx, real pty);

	real length() const;

	real length2() const;

	real distance(const GPoint2d& rhs) const;

	void normalize();

	GPoint2d normalized() const;

	GPoint2d operator-() const;

	GPoint2d& operator+=(const GPoint2d& rhs);

	GPoint2d& operator-=(const GPoint2d& rhs);

	GPoint2d& operator*=(real scale);

	GPoint2d& operator/=(real scale);

	GPoint2d operator+(const GPoint2d& rhs) const;

	GPoint2d operator-(const GPoint2d& rhs) const;

	GPoint2d operator*(real scale) const;

	GPoint2d operator/(real scale) const;

public:
	real x;
	real y;
};

using GVector2d = GPoint2d;

class EXPORT_GEOM GDirection2d : public Geometry
{
public:
	GDirection2d();

	GDirection2d(real ptx, real pty);

	GDirection2d(const GVector2d& vec);

	GDirection2d(const GVector2d &from, const GVector2d& to);

	GDirection2d operator-() const;

	GVector2d scale(real length) const;

	GDirection2d ccwRotate90() const;

	GDirection2d cwRotate90() const;

public:
	real x;
	real y;
};
}
