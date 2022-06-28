#version 460 core
layout (quads, equal_spacing, ccw) in;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 Normal;
out vec3 FragPos;


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

    float u0 = (1.0 - u)*(1.0 - u)*(1.0 - u);
    float u1 = 3.0 * (1.0 - u)*(1.0 - u)*u;
    float u2 = 3.0 * (1.0 - u)*u*u;
    float u3 = u*u*u;

    float v0 = (1.0 - v)*(1.0 - v)*(1.0 - v);
    float v1 = 3.0 * (1.0 - v)*(1.0 - v)*v;
    float v2 = 3.0 * (1.0 - v)*v*v;
    float v3 = v*v*v;

    // calc tangent vector
    float du0 = -3 * (1.0 - u)*(1.0 - u);
    float du1 = 9 * u*u - 12 * u + 3;
    float du2 = 6 * u - 9 * u*u;
    float du3 = 3 * u*u;

    float dv0 = -3 * (1.0 - v)*(1.0 - v);
    float dv1 = 9 * v*v - 12 * v + 3;
    float dv2 = 6 * v - 9 * v*v;
    float dv3 = 3 * v*v;

    vec4 pos = u0 * (p00*v0 + p01*v1 + p02*v2 + p03*v3)
             + u1 * (p10*v0 + p11*v1 + p12*v2 + p13*v3)
             + u2 * (p20*v0 + p21*v1 + p22*v2 + p23*v3)
             + u3 * (p30*v0 + p31*v1 + p32*v2 + p33*v3);

    vec4 tangentU = du0 * (p00*v0 + p01*v1 + p02*v2 + p03*v3)
                  + du1 * (p10*v0 + p11*v1 + p12*v2 + p13*v3)
                  + du2 * (p20*v0 + p21*v1 + p22*v2 + p23*v3)
                  + du3 * (p30*v0 + p31*v1 + p32*v2 + p33*v3);
    vec4 tangentV = u0 * (p00*dv0 + p01*dv1 + p02*dv2 + p03*dv3)
                  + u1 * (p10*dv0 + p11*dv1 + p12*dv2 + p13*dv3)
                  + u2 * (p20*dv0 + p21*dv1 + p22*dv2 + p23*dv3)
                  + u3 * (p30*dv0 + p31*dv1 + p32*dv2 + p33*dv3);
    vec3 aNormal = normalize(cross(tangentU.xyz, tangentV.xyz));

    gl_Position = projection * view * model * pos;
    //Normal Matrix
    Normal = mat3(transpose(inverse(model))) * vec3(0,0,1);
    FragPos = vec3(model * pos);
}
