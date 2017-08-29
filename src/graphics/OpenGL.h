#pragma once

#include "../graphics/glad/glad.h"
#include "../util/Defines.h"

namespace igad
{
#ifdef DEBUG

// Convert a GLenum to a readable c-string
inline const char* GLErrorString(GLenum error)
{
	switch (error)
	{
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";

	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";

	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";

	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";

	default:
		return "UNKNOWN";
	}
}



#define GL_GET_ERROR()                                              \
{                                                                   \
    GLenum error;                                                   \
    bool err = false;                                               \
    while ( (error = glGetError()) != GL_NO_ERROR)                  \
    {                                                               \
        LOG( "OpenGL ERROR: %s\nCHECK POINT: %s (line %d)\n",       \
        igad::GLErrorString(error), __FILE__, __LINE__ );			\
        err = true;                                                 \
    }                                                               \
    ASSERT(!err);                                                   \
}

#define GL_CLEAR_ERROR()                                            \
{                                                                   \
    GLenum error;                                                   \
    while ( (error = glGetError()) != GL_NO_ERROR);                 \
}

#else
#define GL_CLEAR_ERROR()
#define GL_GET_ERROR()
#define LOG(...) { }
#endif

} // igad
