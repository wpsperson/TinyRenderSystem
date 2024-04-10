#include "Geometry/GPoint3d.h"

#include <cmath>

namespace geom
{
GPoint3d::GPoint3d() : x(0.0), y(0.0), z(0.0)
{
}

GPoint3d::GPoint3d(real ptx, real pty, real ptz) : x(ptx), y(pty), z(ptz)
{
}

GPoint3d::~GPoint3d()
{
}

void GPoint3d::setX(real ptx)
{
	x = ptx;
}

void GPoint3d::setY(real pty)
{
	y = pty;
}

void GPoint3d::setZ(real ptz)
{
	z = ptz;
}

void GPoint3d::setValue(real ptx, real pty, real ptz)
{
	x = ptx;
	y = pty;
	z = ptz;
}

real GPoint3d::length() const
{
	return std::sqrt(x * x + y * y);
}

real GPoint3d::length2() const
{
	return x * x + y * y;
}

real GPoint3d::distance(const GPoint3d& rhs) const
{
	GPoint3d vec = (rhs - *this);
	return vec.length();
}

void GPoint3d::normalize()
{
	real len = length();
	if (len == 0)
	{
		return;
	}
	x /= len;
	y /= len;
	z /= len;
}

GPoint3d GPoint3d::normalized() const
{
	real len = length();
	if (len == 0)
	{
		return GPoint3d(1.0, 0.0, 0.0);
	}
	return GPoint3d(x / len, y / len, z / len);
}

GPoint3d GPoint3d::operator-() const
{
	return GPoint3d(-x, -y, -z);
}

GPoint3d& GPoint3d::operator+=(const GPoint3d& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

GPoint3d& GPoint3d::operator-=(const GPoint3d& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

GPoint3d& GPoint3d::operator*=(real scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	return *this;
}

GPoint3d& GPoint3d::operator/=(real scale)
{
	x /= scale;
	y /= scale;
	z /= scale;
	return *this;
}

GPoint3d GPoint3d::operator+(const GPoint3d& rhs) const
{
	return GPoint3d(x + rhs.x, y + rhs.y, z + rhs.z);
}

GPoint3d GPoint3d::operator-(const GPoint3d& rhs) const
{
	return GPoint3d(x - rhs.x, y - rhs.y, z - rhs.z);
}

GPoint3d GPoint3d::operator*(real scale) const
{
	return GPoint3d(x * scale, y * scale, z * scale);
}

GPoint3d GPoint3d::operator/(real scale) const
{
	return GPoint3d(x / scale, y / scale, z / scale);
}



GDirection3d::GDirection3d() : x(1.0), y(0.0), z(0.0)
{
}

GDirection3d::GDirection3d(real ptx, real pty, real ptz) : x(1.0), y(0.0), z(0.0)
{
	real len = std::sqrt(ptx * ptx + pty * pty + ptz * ptz);
	if (len > 0)
	{
		x = ptx / len;
		y = pty / len;
		z = ptz / len;
	}
}

GDirection3d::GDirection3d(const GVector3d& vec) : GDirection3d(vec.x, vec.y, vec.z)
{
}

GDirection3d::GDirection3d(const GVector3d& from, const GVector3d& to) : GDirection3d(to - from)
{
}

GDirection3d GDirection3d::operator-() const
{
	return GDirection3d(-x, -y, -z);
}

GVector3d GDirection3d::scale(real length) const
{
	return GVector3d(x * length, y * length, z * length);
}

}
