#pragma once
#include "glad.h"
#include <cstdarg>
#include <cstddef>

/// @brief Create boilerplate vertex buffer filled with provided data and setup attribute pointers
/// @param data vertex data
/// @param length number of VertexData in data
/// @param type GL_STATIC_DRAW, GL_STATIC_READ, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, ...
/// @param ... lists the number of float in each attrib field. SUM(...)*sizeof(float) must equal sizeof(VertexData)
/// @return Newly created VBO
template <typename VertexData>
GLuint initFloatVertexBuffer(VertexData *data, size_t length, GLenum type,...)
{
    constexpr size_t DATA_SIZE = sizeof(VertexData);
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, DATA_SIZE * length, data, type);
    size_t sum = 0;
    va_list args;
    va_start(args,type);
    for (size_t index = 0; sum < DATA_SIZE; ++index)
    {
        size_t my_size = va_arg(args,size_t);
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index,my_size,GL_FLOAT,GL_FALSE,DATA_SIZE,(void*)sum);
        sum+=my_size*sizeof(float);
    }
    va_end(args);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    return VBO;
}