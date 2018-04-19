#pragma once


namespace CommonLibOgl
{
	// Simple camera: constant up vector, looks at the coordinates origin.
	class Camera
	{
	public:
		// Parameters: position    - camera position in the scene;
		//             aspectRatio - aspect ratio of the main application window;
		//             scaleFactor - scale factor for the camera;
		//             fieldOfView - field of view angle;
		//             frustumNear - near frustum boundary;
		//             frustumFar  - far frustum boundary.
		Camera(glm::vec3 position, GLfloat aspectRatio, GLfloat scaleFactor = 1.0f,
			GLfloat fieldOfView = 45.0f, GLfloat frustumNear = 0.1f, GLfloat frustumFar = 1000.0f);

		virtual ~Camera();

		glm::mat4 getProjectionMatrix() const;

		glm::mat4 getModelMatrix() const;

		glm::mat4 getViewMatrix() const;

		glm::mat4 getModelViewMatrix() const;

		glm::mat4 getModelViewProjectionMatrix() const;

		// React on the resizing of the main application window.
		// Parameters: aspectRatio - aspect ratio of the main application window.
		void resize(GLfloat aspectRatio);

		// Get camera position in the scene.
		glm::vec3 getPosition() const;

		void translateX(GLfloat diff);
		void translateY(GLfloat diff);
		void translateZ(GLfloat diff);

		void rotateX(GLfloat angleDegrees);
		void rotateY(GLfloat angleDegrees);
		void rotateZ(GLfloat angleDegrees);

		void rotateXY(GLfloat xAngleDegrees, GLfloat yAngleDegrees);

		GLfloat getScale() const;

		void scale(GLfloat amount);

	private:
		Camera(const Camera&);
		Camera& operator=(const Camera&);

	private:
		// Initial camera position in the scene.
		const glm::vec3 InitialPosition;

		// Up and front directions.
		const glm::vec3 Up;
		const glm::vec3 Front;

		// Aspect ratio of the window.
		GLfloat m_aspectRatio;

		// Minimum scale factor.
		const GLfloat ScaleFactorMin;

		// Current scale factor.
		GLfloat m_scaleFactor;

		// Field of view angle.
		const GLfloat FieldOfView;

		// Frustum boundaries.
		const GLfloat FrustumNear;
		const GLfloat FrustumFar;

		// Translation vector of the camera.
		glm::vec3 m_translation;

		// Rotation vector (in degrees).
		glm::vec3 m_rotationDegrees;
	};
}
