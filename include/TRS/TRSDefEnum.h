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

enum class ShaderType : unsigned char
{
    None = 0,
    Default,        // only has vertex
    Phong,          // vertex and normal
    PhongTexture,   // vertex normal uv, texture
    DualTexture,    // vertex uv, tex1 tex2
    FontShader,
    CustomShader,
};

enum RenderMode : unsigned char
{
    Shaded = 1,
    WireFrame = 2,
    Points = 4,
};


enum MeshStruct : unsigned char
{
    msVertex = 1,
    msNormal = 2,
    msUV = 4,
    msColor = 8,
};

enum VertexAttributeIndex : unsigned char
{
    VertexIndex = 0,
    NormalIndex = 1,
    TextureCoordIndex0 = 2,
    ColorIndex = 3,
    TextureCoordIndex1,
    TextureCoordIndex2,
};

enum class NodeType : unsigned char
{
    ntUnknow = 0,
    ntGeode = 1,
    ntGroup,
    ntTextNode,
    ntDynamicText,
};

enum class ProjectionMode : unsigned char
{
    Parallel = 0,
    Perspective = 1,
};