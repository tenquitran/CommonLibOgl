#pragma once


namespace CommonLibOgl
{
	// Visible light source (a cube).
	// Uses data from
	// https://github.com/daw42/glslcookbook/blob/master/ingredients/vbocube.cpp
	class LightSourceVisible
	{
	public:
		// Parameters: cubeSide - size of the light source cube side;
		//             color - light color.
		LightSourceVisible(const Camera& camera, float cubeSide, const glm::vec3& color);

		virtual ~LightSourceVisible();

		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		void updateViewMatrices() const;

		void render() const;

	private:
		LightSourceVisible(const LightSourceVisible&) = delete;
		LightSourceVisible& operator=(const LightSourceVisible&) = delete;

	private:
		const Camera& m_camera;

		const float CubeSide;
		const glm::vec3 Color;

		GLuint m_vao;
		GLuint m_vbo;

		GLuint m_index;          // index buffer
		GLsizei m_indexCount;    // number of indices

		std::unique_ptr<CommonLibOgl::ProgramGLSL> m_spProgram;
	};
}
