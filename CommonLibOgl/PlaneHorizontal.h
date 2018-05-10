#pragma once


namespace CommonLibOgl
{
	class PlaneHorizontal
		: public Renderable
	{
	public:
		PlaneHorizontal(GLuint program, Camera& camera, const glm::vec3& center, GLfloat side, const MaterialPhong& material);

		virtual ~PlaneHorizontal();

		virtual void render() const override;

	private:
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
