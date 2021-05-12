#version 460 core
layout (quads, equal_spacing, ccw) in;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// function to calculate the hermite surface function value
vec4 BezierSurface(float u, float v,
    vec4 p00, vec4 p10, vec4 p20, vec4 p30,
    vec4 p01, vec4 p11, vec4 p21, vec4 p31,
    vec4 p02, vec4 p12, vec4 p22, vec4 p32,
    vec4 p03, vec4 p13, vec4 p23, vec4 p33)
{
    float u0 = (1.0-u)*(1.0 - u)*(1.0 - u);
    float u1 = 3.0 * (1.0 - u)*(1.0 - u)*u;
    float u2 = 3.0 * (1.0 - u)*u*u;
    float u3 = u*u*u;

    float v0 = (1.0-v)*(1.0 - v)*(1.0 - v);
    float v1 = 3.0 * (1.0 - v)*(1.0 - v)*v;
    float v2 = 3.0 * (1.0 - v)*v*v;
    float v3 = v*v*v;

    vec4 p =  u0 * (p00*v0 + p01*v1 + p02*v2 + p03*v3)
			+ u1 * (p10*v0 + p11*v1 + p12*v2 + p13*v3)
			+ u2 * (p20*v0 + p21*v1 + p22*v2 + p23*v3)
			+ u3 * (p30*v0 + p31*v1 + p32*v2 + p33*v3);
    return p;
}

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 p00 = gl_in[0 ].gl_Position;
    vec4 p10 = gl_in[1 ].gl_Position;
    vec4 p20 = gl_in[2 ].gl_Position;
    vec4 p30 = gl_in[3 ].gl_Position;
    vec4 p01 = gl_in[4 ].gl_Position;
    vec4 p11 = gl_in[5 ].gl_Position;
    vec4 p21 = gl_in[6 ].gl_Position;
    vec4 p31 = gl_in[7 ].gl_Position;
    vec4 p02 = gl_in[8 ].gl_Position;
    vec4 p12 = gl_in[9 ].gl_Position;
    vec4 p22 = gl_in[10].gl_Position;
    vec4 p32 = gl_in[11].gl_Position;
    vec4 p03 = gl_in[12].gl_Position;
    vec4 p13 = gl_in[13].gl_Position;
    vec4 p23 = gl_in[14].gl_Position;
    vec4 p33 = gl_in[15].gl_Position;
	
	
	vec4 pos = BezierSurface(u, v, 
	    p00, p10, p20, p30,
	    p01, p11, p21, p31,
        p02, p12, p22, p32,
	    p03, p13, p23, p33);

    gl_Position = projection * view * model * pos;
}
