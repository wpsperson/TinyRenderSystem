#version 460 core
layout (vertices = 16) out;

void main()
{
    gl_TessLevelOuter[0] = 10;
    gl_TessLevelOuter[1] = 10;
    gl_TessLevelOuter[2] = 10;
    gl_TessLevelOuter[3] = 10;
    gl_TessLevelInner[0] = 10;
    gl_TessLevelInner[1] = 10;
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
