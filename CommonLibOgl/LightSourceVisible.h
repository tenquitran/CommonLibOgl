#pragma once


namespace CommonLibOgl
{
	// Visible light source (a cube).
	// Uses vertex and index data from
	// https://github.com/daw42/glslcookbook/blob/master/ingredients/vbocube.cpp
	class LightSourceVisible
		: public Renderable
		//, public Movable
	{
	public:
		// Parameters: cubeSide - size of the light source cube side;
		//             position - position in the scene;
		//             side - side length;
		//             color - light color.
		LightSourceVisible(Camera& camera, const glm::vec4& position, GLfloat side, const glm::vec3& color);

		virtual ~LightSourceVisible();

		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		virtual void updateMatrices() const override;

		virtual void render() const override;

	private:
		// Side of the cube which is the visual representation of the light source.
		const GLfloat m_side;

		// For simplicity, the light source has only the emissive color.
		const glm::vec3 m_colorEmissive;

		GLuint m_vao;
		GLuint m_vbo;

		GLuint m_index;          // index buffer
		GLsizei m_indexCount;    // number of indices

		std::unique_ptr<CommonLibOgl::ProgramGLSL> m_spProgram;
	};
}
