#pragma once


namespace CommonLibOgl
{
	// Visible light source (a cube).
	// Uses vertex and index data from
	// https://github.com/daw42/glslcookbook/blob/master/ingredients/vbocube.cpp
	class LightSourceVisible
		: public Renderable
	{
	public:
		// Parameters: cubeSide - size of the light source cube side;
		//             color - light color.
		LightSourceVisible(const Camera& camera, float cubeSide, const glm::vec3& color);

		virtual ~LightSourceVisible();

		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		virtual void updateViewMatrices() const override;

		virtual void render() const override;

	private:
		LightSourceVisible(const LightSourceVisible&) = delete;
		LightSourceVisible& operator=(const LightSourceVisible&) = delete;

	private:
		const Camera& m_camera;

		const float m_cubeSide;

		// For simplicity, the light source has only the emissive color.
		const glm::vec3 m_colorEmissive;

		GLuint m_vao;
		GLuint m_vbo;

		GLuint m_index;          // index buffer
		GLsizei m_indexCount;    // number of indices

		std::unique_ptr<CommonLibOgl::ProgramGLSL> m_spProgram;
	};
}
