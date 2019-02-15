#pragma once


namespace CommonLibOgl
{
	// Simple camera: constant up vector, looks at the coordinates origin.
	class Camera
	{
	public:
		// Parameters: position    - camera position in the scene;
		//             scaleFactor - scale factor for the camera;
		//             fieldOfView - field of view angle;
		//             frustumNear - near frustum boundary;
		//             frustumFar  - far frustum boundary.
		Camera(glm::vec3 position, GLfloat scaleFactor = 1.0f,
			GLfloat fieldOfView = 45.0f, GLfloat frustumNear = 0.1f, GLfloat frustumFar = 1000.0f);

        void translate(const glm::vec3& diff);

        void rotate(const glm::vec3& degrees);

        void scale(GLfloat amount);

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

        // Get current scale factor.
        GLfloat getScale() const;

	private:
		Camera(const Camera&);
		Camera& operator=(const Camera&);

	private:
		// Initial camera position in the scene.
		const glm::vec3 InitialPosition;

		// Up and front directions.
        const glm::vec3 Up = { 0.0f, 1.0f, 0.0f };
		const glm::vec3 Front;

		// Aspect ratio of the window.
        GLfloat m_aspectRatio = { 1.0f };

		// Minimum scale factor.
        const GLfloat ScaleFactorMin = { 0.01f };

		// Current scale factor.
        GLfloat m_scaleFactor = { 1.0f };

		// Field of view angle.
        const GLfloat FieldOfView = { 45.0f };

		// Frustum boundaries.
        const GLfloat FrustumNear = { 0.1f };
        const GLfloat FrustumFar = { 1000.0f };

		// Translation vector of the camera.
		glm::vec3 m_translation;

		// Rotation vector (in degrees).
		glm::vec3 m_rotationDegrees;
	};

    // OpenGL camera: shared_ptr.
    typedef std::shared_ptr<Camera> CameraPtr;
}
