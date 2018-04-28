#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <cassert>

#include <atlstr.h>


// GLEW library (http://glew.sourceforge.net/)
#include <GL/glew.h>
#include <GL/wglew.h>
#pragma comment(lib, "glew32.lib")


// GLM library (http://glm.g-truc.net/)
#pragma warning(push) 
#pragma warning(disable : 4201)
#define GLM_FORCE_RADIANS    // enforce all angles to be in radians, not in degrees
#include <glm/glm.hpp>       // the most common GLM headers
#include <glm/gtc/matrix_transform.hpp>    // matrix transformations: glm::perspective(), etc.
#include <glm/gtc/type_ptr.hpp>            // glm::value_ptr() to pass matrices to shaders, etc.
#include <glm/gtx/euler_angles.hpp>        // glm::yawPitchRoll()
#pragma warning(pop)


// WGL (Windows OpenGL)
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")


// SOIL library.
#include <soil/src/SOIL.h>
#pragma comment(lib, "SOIL.lib")


#include "Exception.h"
#include "WindowInfo.h"
#include "OpenGLInfo.h"
#include "Camera.h"
#include "WindowBase.h"
#include "ProgramGLSL.h"


// Materials.
#include "MaterialPhong.h"


// Commonly used graphics primitives, light sources, etc.
#include "Renderable.h"
#include "LightSourceVisible.h"
#include "Cube.h"
#include "Cubemap.h"
#include "PlaneHorizontal.h"


namespace CommonLibOgl
{
// For OpenGL code.
#define BUFFER_OFFSET(offset) ((void *)(offset))
}
