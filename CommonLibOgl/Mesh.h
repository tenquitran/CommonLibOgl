#pragma once


namespace CommonLibOgl
{
	// Base class for meshes.
	class Mesh
	{
	public:
		explicit Mesh(const glm::vec3& origin);

		virtual ~Mesh();

		virtual void render() const = 0;

	private:
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

	protected:
		GLuint m_vao = {};
		GLuint m_vbo = {};

		GLuint m_index = {};          // index buffer
		GLsizei m_indexCount = {};    // number of indices

		GLuint m_normal = {};         // normal buffer

		glm::vec3 m_origin;
	};
}
