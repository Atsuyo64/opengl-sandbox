#include "openglUtils/texture.hpp"
#include "texture.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(unsigned width, unsigned height)
{
    has_mipmap = false;
    glGenTextures(1, &id);
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float red[] = {1.0f, 0.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, red);
    unbind();
}

/// @brief Generate texture with data
/// @param width
/// @param height
/// @param data_ptr
/// @param format GL_RGB or GL_RGBA
Texture::Texture(unsigned width, unsigned height, unsigned char const *data_ptr, GLenum format) : Texture(width, height)
{
    if (data_ptr != NULL)
    {
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data_ptr);
        unbind();
    }
}

Texture::Texture(char const *path)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cerr << "Image not loaded: " << path << std::endl;
        throw new std::runtime_error(std::string("Could not load image \"") + path + '"');
    }
    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 2)
        format = GL_RG;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    else
        throw new std::runtime_error(std::string("Unsupported number of channels: ") + std::to_string(nrChannels));
    glGenTextures(1, &id);
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float red[] = {1.0f, 0.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, red);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    unbind();
    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::bind()
{
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_texture_id);
    glBindTexture(GL_TEXTURE_2D, id);
}
void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, prev_texture_id);
}

void Texture::bind_to_sampler(unsigned sampler_number)
{
    glActiveTexture(GL_TEXTURE0 + sampler_number);
    bind();
}

void Texture::unbind_to_sampler()
{
    glActiveTexture(GL_TEXTURE0);
    unbind();
}

void Texture::generateMipMap()
{
    bind();
    glGenerateMipmap(GL_TEXTURE_2D);
    has_mipmap = true;
    int value;
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &value);
    if (value == GL_NEAREST)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    }
    else if (value == GL_LINEAR)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    }
    else
    {
        std::cerr << "Unknown filter: " << value << std::endl;
    }
    unbind();
}
/// @brief Nearest <=> mirror == False && clamp == False
/// @param mirror
/// @param clamp Clamp to color if color != NULL, else clamp to border
/// @param color Can be NULL
void Texture::set_wrap_x(bool mirror, bool clamp, float const color[4])
{
    bind();
    if (mirror && clamp)
        std::cerr << "Texture set wrap: mirror and clamp cannot be both true" << std::endl;
    if (mirror)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    else if (clamp && color == NULL)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    else if (clamp && color != NULL)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    }
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    unbind();
}
void Texture::set_wrap_y(bool mirror, bool clamp, float const color[4])
{
    bind();
    if (mirror && clamp)
        std::cerr << "Texture set wrap: mirror and clamp cannot be both true" << std::endl;
    if (mirror)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    else if (clamp && color == NULL)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    else if (clamp && color != NULL)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    }
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    unbind();
}
void Texture::set_min_filter(bool linear_filter, bool linear_mipmap)
{
    bind();
    if (has_mipmap)
    {
        if (linear_filter && linear_mipmap)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        else if (linear_filter && !linear_mipmap)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        else if (!linear_filter && linear_mipmap)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    } else {
        if (linear_filter)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    unbind();
}
void Texture::set_mag_filter(bool linear)
{
    if (linear)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

GLuint Texture::getId() const
{
    return id;
}
