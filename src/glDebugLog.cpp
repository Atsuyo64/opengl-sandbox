#include "glDebugLog.hpp"
#include <stdio.h>
#include <signal.h>

static char log_level = 0x85;

static char const *get_source(GLenum source)
{
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        return "[[OpenGL API]]";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        return "[[WINDOW SYSTEM]]";
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        return "[[SHADER COMPILER]]";
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        return "[[THIRD PARTY]]";
    case GL_DEBUG_SOURCE_APPLICATION:
        return "[[USER GENERATED ERROR]]";
    case GL_DEBUG_SOURCE_OTHER:
        return "[[OTHER]]";
    default:
        return "[[/!\\ UNKNOWN SOURCE TYPE]]";
    }
}

static char const *get_type(GLenum type)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        return "ERROR";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        return "DEPRECATED_BEHAVIOR";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        return "UNDEFINED_BEHAVIOR";
    case GL_DEBUG_TYPE_PORTABILITY:
        return "PORTABILITY";
    case GL_DEBUG_TYPE_PERFORMANCE:
        return "PERFORMANCE";
    case GL_DEBUG_TYPE_MARKER:
        return "MARKER";
    case GL_DEBUG_TYPE_PUSH_GROUP:
        return "PUSH_GROUP";
    case GL_DEBUG_TYPE_POP_GROUP:
        return "POP_GROUP";
    case GL_DEBUG_TYPE_OTHER:
        return "OTHER";
    default:
        return "/!\\ UNKNOWN ERROR TYPE";
    }
}

void opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    if (type == 0x8251)
        return;
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        if ((log_level & 0x7f) > 0)
        {
            fprintf(stderr, "[GL DEBUG HIGH] from %s (%s): %s\n", get_source(source), get_type(type), message);
            if (log_level & 0x80)
#ifdef SIGTRAP
                raise(SIGTRAP);
#else
                raise(SIGABRT);
#endif
        }
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        if ((log_level & 0x7f) > 1)
            fprintf(stderr, "[GL DEBUG MEDIUM] from %s (%s): %s\n", get_source(source), get_type(type), message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        if ((log_level & 0x7f) > 2)
            fprintf(stderr, "[GL DEBUG LOW] from %s (%s): %s\n", get_source(source), get_type(type), message);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        if ((log_level & 0x7f) > 3)
            fprintf(stderr, "[GL DEBUG NOTIF] from %s (%s): %s\n", get_source(source), get_type(type), message);
        break;
    default:
        fprintf(stderr, "[GL DEBUG UNKNOWN] from %s (%s): %s\n", get_source(source), get_type(type), message);
    }
    // fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, messages = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

void set_opengl_log_level(char level, bool high_assert)
{
    log_level = level | (high_assert ? 0x80 : 0x0);
}
