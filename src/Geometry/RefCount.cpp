#include "Geometry/RefCount.h"

namespace geom
{
RefCount::RefCount() : m_count(0)
{

}

RefCount::~RefCount()
{
}

int RefCount::increase()
{
    return ++m_count;
}

int RefCount::decrease()
{
    return --m_count;
}

}