#include "Geometry/GPoint2d.h"

#include <cmath>

namespace geom
{
GPoint2d::GPoint2d() : x(0.0), y(0.0)
{
}

GPoint2d::GPoint2d(real ptx, real pty) : x(ptx), y(pty)
{
}

GPoint2d::~GPoint2d()
{
}

void GPoint2d::setX(real ptx)
{
	x = ptx;
}

void GPoint2d::setY(real pty)
{
	y = pty;
}

void GPoint2d::setValue(real ptx, real pty)
{
	x = ptx;
	y = pty;
}

real GPoint2d::length() const
{
	return std::sqrt(x * x + y * y);
}

real GPoint2d::length2() const
{
	return x * x + y * y;
}

real GPoint2d::distance(const GPoint2d& rhs) const
{
	GPoint2d vec = (rhs  - *this);
	return vec.length();
}

void GPoint2d::normalize()
{
	real len = length();
	if (len == 0)
	{
		return;
	}
	x /= len;
	y /= len;
}

GPoint2d GPoint2d::normalized() const
{
	real len = length();
	if (len == 0)
	{
		return GPoint2d(1.0, 0.0);
	}
	return GPoint2d(x / len, y / len);
}

GPoint2d GPoint2d::operator-() const
{
	return GPoint2d(-x, -y);
}

GPoint2d& GPoint2d::operator+=(const GPoint2d& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

GPoint2d& GPoint2d::operator-=(const GPoint2d& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

GPoint2d& GPoint2d::operator*=(real scale)
{
	x *= scale;
	y *= scale;
	return *this;
}

GPoint2d& GPoint2d::operator/=(real scale)
{
	x /= scale;
	y /= scale;
	return *this;
}

GPoint2d GPoint2d::operator+(const GPoint2d& rhs) const
{
	return GPoint2d(x + rhs.x, y + rhs.y);
}

GPoint2d GPoint2d::operator-(const GPoint2d& rhs) const
{
	return GPoint2d(x - rhs.x, y - rhs.y);
}

GPoint2d GPoint2d::operator*(real scale) const
{
	return GPoint2d(x * scale, y * scale);
}

GPoint2d GPoint2d::operator/(real scale) const
{
	return GPoint2d(x / scale, y / scale);
}



GDirection2d::GDirection2d() : x(1.0), y(0.0)
{
}

GDirection2d::GDirection2d(real ptx, real pty) : x(1.0), y(0.0)
{
	real len = std::sqrt(ptx * ptx + pty * pty);
	if (len > 0)
	{
		x = ptx / len;
		y = pty / len;
	}
}

GDirection2d::GDirection2d(const GVector2d& vec) : GDirection2d(vec.x, vec.y)
{
}

GDirection2d::GDirection2d(const GVector2d& from, const GVector2d& to) : GDirection2d(to - from)
{
}

GDirection2d GDirection2d::operator-() const
{
	return GDirection2d(-x, -y);
}

GVector2d GDirection2d::scale(real length) const
{
	return GVector2d(x * length, y * length);
}

GDirection2d GDirection2d::ccwRotate90() const
{
	return GDirection2d(-y, x);
}

GDirection2d GDirection2d::cwRotate90() const
{
	return GDirection2d(y, -x);
}


}
