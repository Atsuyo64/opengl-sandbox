#pragma once
#include "glad.h"

class Texture{
public:
    Texture(unsigned width,unsigned height);
    Texture(unsigned width,unsigned height, unsigned char const* data_ptr, GLenum format);
    Texture(char const* path);
    Texture(Texture const &) = delete;
    Texture(Texture &&) = delete;
    Texture operator=(Texture const &) = delete;
    Texture operator=(Texture &&) = delete;
    ~Texture();
    
    void bind();
    void unbind() const;
    void bind_to_sampler(unsigned sampler_number);
    void unbind_to_sampler();
    void generateMipMap();
    void set_wrap_x(bool mirror, bool clamp, float const color[4]);
    void set_wrap_y(bool mirror, bool clamp, float const color[4]);
    void set_min_filter(bool linear_filter,bool linear_mipmap);
    void set_mag_filter(bool linear);
    
    GLuint getId() const;
    
private:
    bool has_mipmap = false;
    GLuint id;
    GLint prev_texture_id;
};