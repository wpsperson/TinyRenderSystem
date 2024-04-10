#pragma once

#include "Geometry/Geometry.h"

namespace geom
{
class EXPORT_GEOM GCurve2d : public Geometry
{
public:

protected:
	Range param;
};
}