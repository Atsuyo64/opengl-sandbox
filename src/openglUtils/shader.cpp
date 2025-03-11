#include "shader.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include "glm/gtc/type_ptr.hpp"

static GLint compile(char const *path, GLenum type)
{
    char *string = nullptr;
    int length;
    try
    {
        std::ifstream file;
        file.exceptions(std::ios::badbit | std::ios::failbit); // | std::ios::ate
        file.open(path, std::ios::binary | std::ios::ate);
        length = file.tellg();
        file.seekg(0, std::ios::beg);
        string = new char[length];
        file.read(string, length);
        file.close();
    }
    catch (const std::ifstream::failure &e)
    {
        if (string != nullptr)
            delete[] string;
        std::cerr << "ERROR::SHADER: Could not open file " << path << ": " << e.what() << std::endl;
        return 0;
    }
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &string, &length);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        char msg[1024];
        GLint size;
        glGetShaderInfoLog(shader, 1024, &size, msg);
        std::cerr << "ERROR::SHADER: Compilation error of " << path << ": " << msg << std::endl;
        return 0;
    }
    delete[] string;
    return shader;
}

Shader::Shader(char const *vertex_shader_path, char const *fragment_shader_path)
{
    GLint vertex_shader = compile(vertex_shader_path, GL_VERTEX_SHADER);
    GLint fragment_shader = compile(fragment_shader_path, GL_FRAGMENT_SHADER);
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
    {
        char msg[1024];
        GLint size;
        glGetShaderInfoLog(program, 1024, &size, msg);
        std::cerr << "ERROR::SHADER: Linking error: " << msg << std::endl;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::Shader(char const *vertex_shader_path, char const *geometry_shader_path, char const *fragment_shader_path)
{
    GLint vertex_shader = compile(vertex_shader_path, GL_VERTEX_SHADER);
    GLint geometry_shader = compile(geometry_shader_path, GL_GEOMETRY_SHADER);
    GLint fragment_shader = compile(fragment_shader_path, GL_FRAGMENT_SHADER);
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, geometry_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
    {
        char msg[1024];
        GLint size;
        glGetShaderInfoLog(program, 1024, &size, msg);
        std::cerr << "ERROR::SHADER: Linking error: " << msg << std::endl;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(geometry_shader);
    glDeleteShader(fragment_shader);
}

void Shader::use() const
{
    glUseProgram(program);
}

void Shader::unuse() const
{
    glUseProgram(0);
}

Shader::~Shader()
{
    glDeleteProgram(program);
}

void Shader::setUniformMatrix4x4(char const *name, glm::mat4 const &value)
{
    // FIXME: store location
    GLint location = glGetUniformLocation(program, name);
    if (location != -1)
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniformInt(char const *name, int const value)
{
    // FIXME: store location
    GLint location = glGetUniformLocation(program, name);
    if (location != -1)
        glUniform1i(location, value);
    else
        printf("Int not found\n");
}

GLint Shader::get_program_ID() const
{
    return program;
}
