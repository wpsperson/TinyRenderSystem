#pragma once
#include <string>

namespace GLSL
{
    // common shader
    const std::string header_version = "#version 330 core";
    const std::string endline = "\n";
    const std::string tokenColor = "#COLOR#";
    const std::string tokenIndex = "#INDEX#";


    // vertex shader
    const std::string define_attribute_pos =    "layout(location = 0) in vec3 aPos;";
    const std::string define_attribute_normal = "layout(location = 1) in vec3 aNormal;";
    const std::string define_attribute_uv =     "layout(location = 2) in vec2 aUV;";
    const std::string define_attribute_color =  "layout(location = 3) in vec3 aColor;";

    const std::string out_normal = "out vec3 Normal;";
    const std::string out_uv = "out vec2 UV;";
    const std::string out_color = "out vec3 Color;";
    const std::string out_fragpos = "out vec3 FragPos;";
    const std::string def_uniform_model = "uniform mat4 model;";
    const std::string def_uniform_viewproj = "uniform mat4 view_proj;";

    const std::string main_begin = "void main()\n{\n";
    const std::string main_end = "}\n";

    const std::string calc_gl_position1 = "gl_Position = vec4(aPos, 1.0);";
    const std::string calc_gl_position2 = "gl_Position = view_proj * model *  vec4(aPos, 1.0);";
    const std::string calc_normal = "Normal = mat3(transpose(inverse(model))) * aNormal;";
    const std::string calc_uv = "UV = aUV;";
    const std::string calc_color = "Color = aColor;";
    const std::string calc_fragpos = "FragPos = vec3(model * vec4(aPos, 1.0));";

    // fragment shader
    const std::string out_fragcolor = "out vec4 FragColor;";
    const std::string in_normal = "in vec3 Normal;";
    const std::string in_uv = "in vec2 UV;";
    const std::string in_color = "in vec3 Color;";
    const std::string in_fragpos = "in vec3 FragPos;";

    const std::string def_uniform_basecolor = "uniform vec4 baseColor;";
    const std::string def_uniform_viewpos = "uniform vec3 viewPos;";
    const std::string def_uniform_lightpos = "uniform vec3 lightPos;";
    const std::string def_uniform_texture = "uniform sampler2D texture#INDEX#;";

    const std::string calc_ambient = "vec3 ambient = vec3(#COLOR#) * 0.2;";
    const std::string calc_diffuse_part1 = "vec3 norm = normalize(Normal);";
    const std::string calc_diffuse_part2 = "vec3 lightDir = normalize(FragPos - lightPos);"; // direction of light go forward.
    const std::string calc_diffuse_part3 = "float diff = max(dot(norm, -lightDir), 0.0);";
    const std::string calc_diffuse_part4 = "vec3 diffuse = diff * vec3(#COLOR#) * 0.5;";
    const std::string calc_specular_part1 = "vec3 fragToCameraDir = normalize(viewPos - FragPos);";
    const std::string calc_specular_part2 = "vec3 reflectDir = reflect(lightDir, norm);";
    const std::string calc_specular_part3 = "float spec = pow(max(dot(fragToCameraDir, reflectDir), 0.0), 32);";
    const std::string calc_specular_part4 = "vec3 specular = (spec * vec3(1.0,1.0,1.0)) * 0.3;";

    const std::string calc_texture0 = "vec4 color0 = texture(texture0, UV);";
    const std::string calc_texture1 = "vec4 color1 = texture(texture1, UV);";
    const std::string calc_special_alpha =
                                            "if(color0.a == 0.0)\n"
                                            "{\n"
                                            "    FragColor = color1;\n"
                                            "    return;\n"
                                            "}\n"
                                            "else if(color1.a== 0.0)\n"
                                            "{\n"
                                            "    FragColor = color0;\n"
                                            "    return;\n"
                                            "}";


    const std::string calc_fragcolor1 = "FragColor = baseColor;";
    const std::string calc_fragcolor2 = "FragColor =  vec4(ambient + diffuse + specular, 1.0);";
    const std::string calc_fragcolor3 = "FragColor = texture(texture0, UV);";
    const std::string calc_fragcolor3_mix = "FragColor = mix(color0, color1, 0.5);";
    const std::string calc_fragcolor4 = "FragColor = vec4(baseColor.xyz, texture(texture0, UV).r);";

}


