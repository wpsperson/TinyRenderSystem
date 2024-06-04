#pragma once


enum class DrawType : unsigned char
{
    POINTS = 0,
    LINES = 1,
    LINE_LOOP = 2,
    LINE_STRIP = 3,
    TRIANGLES = 4,
    TRIANGLE_STRIP = 5,
    TRIANGLE_FAN = 6,
    PATCHES = 14,
};

enum VertexAttributeIndex
{
    VertexIndex = 0,
    NormalIndex = 1,
    TextureCoordIndex0 = 2,
    ColorIndex = 3,
    TextureCoordIndex1,
    TextureCoordIndex2,
};

enum MeshStruct
{
    msVertex = 1,
    msNormal = 2,
    msUV = 4,
    msColor = 8,
};
