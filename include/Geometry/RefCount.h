#pragma once

#include "Geometry/Export.h"

namespace geom
{
class EXPORT_GEOM RefCount
{
public:
    RefCount();

    ~RefCount();

    int increase();

    int decrease();

private:
    int m_count;
};
}
