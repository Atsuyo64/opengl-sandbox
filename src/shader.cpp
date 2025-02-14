#include "shader.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

static GLint compile(char *path, GLenum type)
{
    char const *string = nullptr;
    try
    {
        std::ifstream file;
        file.exceptions(std::ios::badbit | std::ios::failbit);
        file.open(path);
        std::stringstream tmp;
        tmp << file.rdbuf();
        string = tmp.str().c_str();
    }
    catch (const std::ifstream::failure &e)
    {
        std::cerr << "ERROR::SHADER: Could not open file " << path << ": " << e.what() << std::endl;
        return 0;
    }
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &string, NULL);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char msg[512];
        GLint size;
        glGetShaderInfoLog(shader,512,&size,msg);
        std::cerr << "ERROR::SHADER: Compilation error of " << path << ": " << msg << std::endl;
    }
    return shader;
}

Shader::Shader(char *vertex_shader_path, char *fragment_shader_path)
{
}
