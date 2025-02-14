#include "shader.hpp"
#include <iostream>
#include <string>
#include <fstream>

static std::string& compile(char* path)
{
    try
    {
        std::ifstream file;
        file.exceptions(std::ios::badbit | std::ios::failbit);
        file.open(path);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

Shader::Shader(char *vertex_shader_path, char *fragment_shader_path)
{

}
