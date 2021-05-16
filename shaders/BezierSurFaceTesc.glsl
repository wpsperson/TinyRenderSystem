#version 460 core
layout (vertices = 16) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //LOD
    //vec4 pos0 = projection * view * model * gl_in[0].gl_Position;
    //vec4 pos15 = projection * view * model * gl_in[15].gl_Position;
    //pos0.xy /= pos0.w;
    //pos15.xy /= pos15.w;
    //float dist = distance(pos15.xy, pos0.xy);
    //float level = 10.0;
    //gl_TessLevelOuter[0] = dist*level;
    //gl_TessLevelOuter[1] = dist*level;
    //gl_TessLevelOuter[2] = dist*level;
    //gl_TessLevelOuter[3] = dist*level;
    //gl_TessLevelInner[0] = dist*level;
    //gl_TessLevelInner[1] = dist*level;

    gl_TessLevelOuter[0] = 10;
    gl_TessLevelOuter[1] = 10;
    gl_TessLevelOuter[2] = 10;
    gl_TessLevelOuter[3] = 10;
    gl_TessLevelInner[0] = 10;
    gl_TessLevelInner[1] = 10;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
