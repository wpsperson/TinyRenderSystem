#version 460 core
layout (isolines, equal_spacing, ccw) in;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// function to calculate the hermite function value
vec3 Bezier(float u, vec3 p0, vec3 p1, vec3 p2, vec3 p3)
{
    float F1 = (1.0-u)*(1.0 - u)*(1.0 - u);
    float F2 = 3.0 * (1.0 - u)*(1.0 - u)*u;
    float F3 = 3.0 * (1.0 - u)*u*u;
    float F4 = u*u*u;

    vec3 p = F1*p0 + F2*p1 + F3*p2 + F4*p3;
    return p;
}

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec3 p0 = vec3(gl_in[0].gl_Position);
    vec3 p1 = vec3(gl_in[1].gl_Position);
    vec3 p2 = vec3(gl_in[2].gl_Position);
    vec3 p3 = vec3(gl_in[3].gl_Position);
    vec3 v3pos = Bezier( u, p0, p1, p2, p3);
    vec4 pos = vec4( v3pos, 1.);
    gl_Position = projection * view * model * pos;
}
