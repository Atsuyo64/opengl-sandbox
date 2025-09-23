#pragma once
#include "glad.h"

class FrameBuffer {
public:
    FrameBuffer();
    FrameBuffer(FrameBuffer const &) = delete;
    FrameBuffer(FrameBuffer &&) = delete;
    FrameBuffer operator=(FrameBuffer const &) = delete;
    FrameBuffer operator=(FrameBuffer &&) = delete;
    void bind();
    void unbind() const;
    ~FrameBuffer();
    bool is_binded() const;
private:
    GLuint fbo;
    GLint prev_fbo_read;
    GLint prev_fbo_write;
    bool check_status_ok();
};