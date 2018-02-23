#pragma once


namespace CommonLibOgl
{
	// OpenGL settings.
	struct OpenGLInfo 
	{
	public:
		OpenGLInfo(int openGlMajor, int openGlMinor, GLfloat fov, GLfloat frustumNear, GLfloat frustumFar)
			: OpenGlMajor(openGlMajor), OpenGlMinor(openGlMinor), FieldOfView(fov), FrustumNear(frustumNear), FrustumFar(frustumFar)
		{
		}

	public:
		// OpenGL version number.
		const int OpenGlMajor;
		const int OpenGlMinor;

		// Field of view angle.
		const GLfloat FieldOfView = 45.0f;

		// Frustum boundaries.
		const GLfloat FrustumNear = 0.1f;
		const GLfloat FrustumFar = 1000.0f;
	};
}
