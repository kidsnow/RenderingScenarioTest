#include "GLHelper.h"

#include "GL/glew.h"
#include <stdio.h>

// GL ERROR CHECK
int CheckGLError(const char* _file, int _line)
{
	int    retCode = 0;

#if !defined(NDEBUG) || defined(_DEBUG) // define 없애지 말것 (릴리즈에서는 그릴때마다 glError체크하면 속도저하생김)
	GLenum glErr;
	glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		const char* sError = nullptr;

		switch (glErr)
		{
		case GL_NO_ERROR:
			sError = "GL_NO_ERROR";
			break;
		case GL_INVALID_ENUM:
			sError = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			sError = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			sError = "GL_INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			sError = "GL_STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			sError = "GL_STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			sError = "GL_OUT_OF_MEMORY";
			break;
		case GL_TABLE_TOO_LARGE:
			sError = "GL_TABLE_TOO_LARGE";
			break;
		}

		if (sError)
			printf("GL Error #%d (%s) in File %s at line: %d \n", glErr, sError, _file, _line);
		else
			printf("GL Error #%d (no message available) in File %s at line: %d \n", glErr, _file, _line);

		if (glErr == GL_STACK_UNDERFLOW || glErr == GL_STACK_OVERFLOW)
		{
			GLint matrixMode;
			GLint stackDepth;
			GLint activeTexture;

			glGetIntegerv(GL_MATRIX_MODE, &matrixMode);

			switch (matrixMode)
			{
			case GL_MODELVIEW:
				glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &stackDepth);
				printf("GL_MODELVIEW, stack(%d) \n", stackDepth);
				break;
			case GL_PROJECTION:
				glGetIntegerv(GL_PROJECTION_STACK_DEPTH, &stackDepth);
				printf("GL_PROJECTION, stack(%d) \n", stackDepth);
				break;
			case GL_TEXTURE:
				glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);
				glGetIntegerv(GL_TEXTURE_STACK_DEPTH, &stackDepth);
				printf("GL_TEXTURE, stack(%d) \n", stackDepth);
				break;
			}
		}

		retCode = 1;
		glErr = glGetError();
	}
#endif
	return retCode;
}