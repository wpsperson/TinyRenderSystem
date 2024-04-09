#pragma once

#include "Geometry/Export.h"

namespace geom
{
class EXPORT_GEOM RefCount
{
public:
    RefCount();

    virtual ~RefCount();

    int increase();

    int decrease();

private:
    mutable int m_count;
};
}
