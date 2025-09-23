#include "openglUtils/frameBuffer.hpp"
#include <iostream>

FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &fbo);
}

void FrameBuffer::bind()
{
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &prev_fbo_write);
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &prev_fbo_read);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prev_fbo_write);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, prev_fbo_read);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &fbo);
}

bool FrameBuffer::is_binded() const
{
    GLint current_fbo_read;
    GLint current_fbo_write;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &current_fbo_write);
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &current_fbo_read);
    return current_fbo_read == fbo && current_fbo_write == fbo;
}

bool FrameBuffer::check_status_ok()
{
    GLenum status; // OpenGL 4.5 => glCheckNamedFrameBufferStatus(fbo,GL_FRAMEBUFFER);
    if (!is_binded())
    {
        bind();
        status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        unbind();
    }
    else
    {
        status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    }
    if (status == GL_FRAMEBUFFER_COMPLETE)
        return true;
    else
    {
        switch (status)
        {
        case GL_FRAMEBUFFER_UNDEFINED:
            std::cerr << "ERROR: Framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist" << std::endl;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cerr << "Error: One+ of the framebuffer attachment points is framebuffer incomplete" << std::endl;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cerr << "Error: Framebuffer does not have at least one image attached to it" << std::endl;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cerr << "Error: GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi" << std::endl;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cerr << "Error: GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER" << std::endl;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cerr << "Error: The combination of internal formats of the attached images violates an implementation-dependent set of restrictions" << std::endl;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            std::cerr << "Error: The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; or the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES; or the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures. " << std::endl;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            std::cerr << "Error: At least one framebuffer attachment is layered, and at least one populated attachment is not layered, or all populated color attachments are not from textures of the same target. " << std::endl;
        default:
            std::cerr << "Error: Unknown frame buffer error..." << std::endl;
        }
        return false;
    }
}
