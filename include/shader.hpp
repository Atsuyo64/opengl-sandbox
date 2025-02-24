#pragma once
#include "glad.h"
#include "glm/mat4x4.hpp"

class Shader
{
public:
    Shader(char const *vertex_shader_path, char const *fragment_shader_path);
    Shader(char const *vertex_shader_path, char const *geometry_shader_path, char const *fragment_shader_path);
    void use() const;

    ~Shader();
    Shader(Shader const &) = delete;
    Shader(Shader &&) = delete;
    Shader operator=(Shader const &) = delete;
    Shader operator=(Shader &&) = delete;
    void setUniformMatrix4x4(char const* name,glm::mat4 const& value);
    // Shader();
    // void from_source(char* vertex_string,char* fragment_string);
    // void from_source(char* vertex_string,char* geometry_string,char* framgment_string);
private:
    GLint program;
};