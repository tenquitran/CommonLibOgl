#pragma once


namespace CommonLibOgl
{
	// Solid-colored cube with Phong shading.
	// Uses vertex, index and normals data from
	// https://github.com/daw42/glslcookbook/blob/master/ingredients/vbocube.cpp
	class Cube
		: public Renderable
		//, public Movable
	{
	public:
		// Parameters: program - GLSL program ID;
		//             camera - scene camera;
		//             position - position in the scene;
		//             side - side length;
		//             material - Phong material properties.
		Cube(GLuint program, Camera& camera, const glm::vec3& position, GLfloat side, const MaterialPhong& material);

		virtual ~Cube();

		virtual void render() const override;

	private:
		Cube(const Cube&) = delete;
		Cube& operator=(const Cube&) = delete;

		void setMaterialProperties() const;

	private:
		const GLfloat m_side;

		const MaterialPhong m_material;

		GLuint m_vao;
		GLuint m_vbo;

		GLuint m_index;          // index buffer
		GLsizei m_indexCount;    // number of indices

		GLuint m_normal;         // normal buffer
	};
}
