#pragma once

#include "Geometry/Geometry.h"

namespace geom
{
class EXPORT_GEOM GPoint3d : public Geometry
{
public:
	GPoint3d();

	GPoint3d(real ptx, real pty, real ptz);

	~GPoint3d();

	void setX(real ptx);

	void setY(real pty);

	void setZ(real ptz);

	void setValue(real ptx, real pty, real ptz);

	real length() const;

	real length2() const;

	real distance(const GPoint3d& rhs) const;

	void normalize();

	GPoint3d normalized() const;

	GPoint3d operator-() const;

	GPoint3d& operator+=(const GPoint3d& rhs);

	GPoint3d& operator-=(const GPoint3d& rhs);

	GPoint3d& operator*=(real scale);

	GPoint3d& operator/=(real scale);

	GPoint3d operator+(const GPoint3d& rhs) const;

	GPoint3d operator-(const GPoint3d& rhs) const;

	GPoint3d operator*(real scale) const;

	GPoint3d operator/(real scale) const;

public:
	real x;
	real y;
	real z;
};

using GVector3d = GPoint3d;


class EXPORT_GEOM GDirection3d : public Geometry
{
public:
	GDirection3d();

	GDirection3d(real ptx, real pty, real ptz);

	GDirection3d(const GVector3d& vec);

	GDirection3d(const GVector3d& from, const GVector3d& to);

	GDirection3d operator-() const;

	GVector3d scale(real length) const;

public:
	real x;
	real y;
	real z;
};

}