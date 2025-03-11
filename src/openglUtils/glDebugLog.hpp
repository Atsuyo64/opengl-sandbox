#pragma once
#include "glad.h"

void opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

// 0 = no log, 1 = high, 2 = medium, 3 = low, 4 = notification
// high_assert = exit on high severity
void set_opengl_log_level(char level,bool high_assert);

//extern int LOG_LEVEL;