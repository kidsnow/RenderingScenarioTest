#pragma once

int CheckGLError(const char* _file, int _line);
#define CHECK_GL_ERROR CheckGLError(__FILE__, __LINE__);
