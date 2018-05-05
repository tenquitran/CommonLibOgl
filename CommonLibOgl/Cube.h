#pragma once


namespace CommonLibOgl
{
	// Solid-colored cube with Phong shading.
	// Uses vertex, index and normals data from
	// https://github.com/daw42/glslcookbook/blob/master/ingredients/vbocube.cpp
	class Cube
		: public Renderable, 
		  public Movable
	{
	public:
		// Parameters: program - GLSL program ID;
		//             camera - scene camera;
		//             position - cube position in the scene;
		//             side - cube side length;
		//             material - Phong material properties.
		Cube(GLuint program, Camera& camera, const glm::vec3& position, GLfloat side, const MaterialPhong& material);

		virtual ~Cube();

		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		virtual void updateMatrices() const override;

		virtual void render() const override;

		void translate(const glm::vec3& diff);

		void rotateX(GLfloat degrees);
		void rotateY(GLfloat degrees);
		void rotateZ(GLfloat degrees);

		void scale(GLfloat amount);

	private:
		Cube(const Cube&) = delete;
		Cube& operator=(const Cube&) = delete;

		void setMaterialProperties() const;

	private:
		const GLuint m_program;    // GLSL program ID

		glm::vec3 m_position;    // position in the scene

		const GLfloat m_side;

		const MaterialPhong m_material;

		GLuint m_vao;
		GLuint m_vbo;

		GLuint m_index;          // index buffer
		GLsizei m_indexCount;    // number of indices

		GLuint m_normal;         // normal buffer
	};
}
