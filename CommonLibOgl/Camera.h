#pragma once


namespace CommonLibOgl
{
#if 1
	class Camera
	{
	public:
		Camera(glm::vec3 position, GLfloat aspectRatio, GLfloat scaleFactor = 1.0f,
			GLfloat fieldOfView = 45.0f, GLfloat frustumNear = 0.1f, GLfloat frustumFar = 1000.0f);

		virtual ~Camera();

		// React on the resizing of the main application window.
		// Parameters: aspectRatio - aspect ratio of the main application window.
		void resize(GLfloat aspectRatio);

		void translateX(GLfloat diff);
		void translateY(GLfloat diff);
		void translateZ(GLfloat diff);

		void rotateX(GLfloat degrees);
		void rotateY(GLfloat degrees);
		void rotateZ(GLfloat degrees);

		void scale(GLfloat amount);

		// In reality, returns only the view matrix.
		glm::mat4 getModelViewMatrix() const;

		glm::mat4 getProjectionMatrix() const;

		glm::mat4 getModelViewProjectionMatrix() const;

	private:
		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;

#if 0
		void updateProperties();
#endif

	private:
		glm::vec3 m_position;

		GLfloat m_aspectRatio;

		GLfloat m_scaleFactor;

		// Minimum scale factor.
		const GLfloat ScaleFactorMin;

		const GLfloat FieldOfView;
		const GLfloat FrustumNear;
		const GLfloat FrustumFar;

#if 0
		GLfloat m_yaw;
		GLfloat m_pitch;
		GLfloat m_roll;
#else
		glm::vec3 m_rotationDegrees;
#endif
	};
#else
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

#if 0
		// Get translation vector of the camera.
		glm::vec3 getTranslation() const;
#else
		// Get camera position in the scene.
		glm::vec3 getPosition() const;
#endif

		void translateX(GLfloat diff);
		void translateY(GLfloat diff);
		void translateZ(GLfloat diff);

		void rotateX(GLfloat angleDegrees);
		void rotateY(GLfloat angleDegrees);
#if 0
		void rotateZ(GLfloat angleDegrees);
#endif

		void rotateXY(GLfloat xAngleDegrees, GLfloat yAngleDegrees);

		GLfloat getScale() const;

		void scale(GLfloat amount);

	private:
		Camera(const Camera&);
		Camera& operator=(const Camera&);

		// Calculate and update the Model-View-Projection matrix.
		void updateMatrices();

	private:
		// Position in the scene.
		glm::vec3 m_position;

		// Up direction for the world.
		const glm::vec3 m_upWorld;

		// Up, front and right directions for the camera.
		glm::vec3 m_up;
		glm::vec3 m_front;
		glm::vec3 m_right;

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

#if 0
		// Translation vector of the camera.
		glm::vec3 m_translation;
#endif

#if 0
		// Rotation vector (in degrees).
		glm::vec3 m_rotationDegrees;
#else
		// Rotation (Euler angles).
		GLfloat m_yaw;
		GLfloat m_pitch;
#endif

		// Model matrix.
		glm::mat4 m_model;

		// View matrix.
		glm::mat4 m_view;

		// Projection matrix.
		glm::mat4 m_projection;

		// Model-View (MV) matrix.
		glm::mat4 m_mv;

		// Model-View-Projection (MVP) matrix.
		glm::mat4 m_mvp;
	};
#endif
}
