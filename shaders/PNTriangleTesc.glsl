#version 460 core
layout (vertices = 3) out;
in vec3 vNormal[];
out vec3 tcNormals[];

void main()
{
    gl_TessLevelOuter[0] = 6;
    gl_TessLevelOuter[1] = 6;
    gl_TessLevelOuter[2] = 6;
    gl_TessLevelInner[0] = 6;

    tcNormals[gl_InvocationID] = vNormal[gl_InvocationID];

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
