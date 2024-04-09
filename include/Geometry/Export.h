#pragma once

#ifdef GEOMETRY_EXPORTS
#define EXPORT_GEOM __declspec(dllexport)
#else
#define EXPORT_GEOM __declspec(dllimport)
#endif
