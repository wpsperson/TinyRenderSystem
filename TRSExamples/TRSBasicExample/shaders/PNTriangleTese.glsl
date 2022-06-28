#version 460 core
layout (triangles, equal_spacing, ccw) in;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 tcNormals[];
out vec3 Normal;
out vec3 FragPos;


void main()
{
    vec3 p1 = gl_in[0].gl_Position.xyz;
    vec3 p2 = gl_in[1].gl_Position.xyz;
    vec3 p3 = gl_in[2].gl_Position.xyz;
    vec3 n1 = tcNormals[0];
    vec3 n2 = tcNormals[1];
    vec3 n3 = tcNormals[2];
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;

    vec3 b300 = p1;
    vec3 b030 = p2;
    vec3 b003 = p3;
    float w12 = dot(p2 - p1, n1);
    float w21 = dot(p1 - p2, n2);
    float w13 = dot(p3 - p1, n1);
    float w31 = dot(p1 - p3, n3);
    float w23 = dot(p3 - p2, n2);
    float w32 = dot(p2 - p3, n3);

    vec3 b210 = (2.*p1 + p2 - w12*n1) / 3.;
    vec3 b120 = (2.*p2 + p1 - w21*n2) / 3.;
    vec3 b021 = (2.*p2 + p3 - w23*n2) / 3.;
    vec3 b012 = (2.*p3 + p2 - w32*n3) / 3.;
    vec3 b102 = (2.*p3 + p1 - w31*n3) / 3.;
    vec3 b201 = (2.*p1 + p3 - w13*n1) / 3.;
    vec3 ee = (b210 + b120 + b021 + b012 + b102 + b201) / 6.;
    vec3 vv = (p1 + p2 + p3) / 3.;
    vec3 b111 = ee + (ee - vv) / 2.;
    vec3 pos = 1.*b300*w*w*w + 1.*b030*u*u*u + 1.*b003*v*v*v +
        3.*b210*u*w*w + 3.*b120*u*u*w + 3.*b201*v*w*w +
        3.*b021*u*u*v + 3.*b102*v*v*w + 3.*b012*u*v*v +
        6.*b111*u*v*w;


    float v12 = 2. * dot(p2 - p1, n1 + n2) / dot(p2 - p1, p2 - p1);
    float v23 = 2. * dot(p3 - p2, n2 + n3) / dot(p3 - p2, p3 - p2);
    float v31 = 2. * dot(p1 - p3, n3 + n1) / dot(p1 - p3, p1 - p3);
    vec3 n200 = n1;
    vec3 n020 = n2;
    vec3 n002 = n3;
    vec3 n110 = normalize(n1 + n2 - v12*(p2 - p1));
    vec3 n011 = normalize(n2 + n3 - v23*(p3 - p2));
    vec3 n101 = normalize(n3 + n1 - v31*(p1 - p3));
    vec3 norm = n200*w*w + n020*u*u + n002*v*v +  n110*w*u + n011*u*v + n101*w*v;

    gl_Position = projection * view * model * vec4(pos, 1.0);
    //Normal Matrix
    Normal = mat3(transpose(inverse(model))) * norm;
    FragPos = vec3(model * vec4(pos, 1.0));
}
