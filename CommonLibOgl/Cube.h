#pragma once


namespace CommonLibOgl
{
	// Solid-colored cube with Phong shading.
	// Uses vertex, index and normals data from
	// https://github.com/daw42/glslcookbook/blob/master/ingredients/vbocube.cpp
	class Cube
		: public Renderable
	{
	public:
		// Parameters: program - GLSL program ID;
		//             camera - scene camera;
		//             side - cube side length;
		//             material - Phong material properties.
		Cube(GLuint program, const Camera& camera, GLfloat side, const MaterialPhong& material);

		virtual ~Cube();

		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		virtual void updateViewMatrices() const override;

		virtual void render() const override;

	private:
		Cube(const Cube&) = delete;
		Cube& operator=(const Cube&) = delete;

	private:
		const GLuint m_program;    // GLSL program ID

		const Camera& m_camera;

		const GLfloat m_cubeSide;

		const MaterialPhong m_material;

		GLuint m_vao;
		GLuint m_vbo;

		GLuint m_index;          // index buffer
		GLsizei m_indexCount;    // number of indices

		GLuint m_normal;         // normal buffer
	};
}
