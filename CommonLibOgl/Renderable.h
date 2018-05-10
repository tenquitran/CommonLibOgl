#pragma once


namespace CommonLibOgl
{
	// Base class for the renderable primitives.
	class Renderable
	{
	public:
		// Constructor for primitives that use external GLSL program (usually from a scene).
		// Parameters: camera - scene camera;
		//             position - position in the scene.
		Renderable(GLuint program, Camera& camera, const glm::vec3& position);

		// Constructor for primitives that will create their own GLSL program.
		// Parameters: camera - scene camera;
		//             position - position in the scene.
		Renderable(Camera& camera, const glm::vec3& position);

		virtual void render() const = 0;

		//////////////////////////////////////////////////////////////////////////
		// Transformation.
		//////////////////////////////////////////////////////////////////////////

		void translate(const glm::vec3& diff);
		glm::vec4 getPosition() const;

		void rotateX(GLfloat degrees);
		void rotateY(GLfloat degrees);
		void rotateZ(GLfloat degrees);

		void scale(GLfloat amount);

		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		// The parent class version assumes the following uniform locations:
		// 0 - MVP
		// 1 - ModelView
		// 2 - Normal
		virtual void updateMatrices() const;

	private:
		Renderable(const Renderable&) = delete;
		Renderable& operator=(const Renderable&) = delete;

	protected:
		// GLSL program ID.
		GLuint m_programId;

		Camera& m_camera;

		// Initial position in the scene. Can be useful if we will need to return to it.
		const glm::vec4 m_posInitial;

		glm::vec3 m_translation;

		glm::vec3 m_rotationDegrees;

		GLfloat m_scaleFactor;
	};
}
