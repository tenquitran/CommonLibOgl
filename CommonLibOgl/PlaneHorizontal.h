#pragma once


namespace CommonLibOgl
{
	class PlaneHorizontal
		: public Renderable
	{
	public:
		PlaneHorizontal(GLuint program, Camera& camera, const glm::vec3& center, GLfloat side, const MaterialPhong& material);

		virtual ~PlaneHorizontal();

		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		virtual void updateMatrices() const override;

		virtual void render() const override;

	private:
		void setMaterialProperties() const;

	private:
		const GLuint m_program;    // GLSL program ID

		glm::vec3 m_center;

		const GLfloat m_side;

		const MaterialPhong m_material;

		GLuint m_vao;
		GLuint m_vbo;

		GLuint m_index;          // index buffer
		GLsizei m_indexCount;    // number of indices

		GLuint m_normal;         // normal buffer
	};
}
