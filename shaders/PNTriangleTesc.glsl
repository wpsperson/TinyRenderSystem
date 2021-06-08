#version 460 core
layout (vertices = 3) out;
in vec3 vNormal[];
out vec3 tcNormals[];
uniform int lod;

void main()
{
    gl_TessLevelOuter[0] = lod;
    gl_TessLevelOuter[1] = lod;
    gl_TessLevelOuter[2] = lod;
    gl_TessLevelInner[0] = lod;

    tcNormals[gl_InvocationID] = vNormal[gl_InvocationID];

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
