#pragma once


namespace CommonLibOgl
{
	// Solid-colored cube with Phong shading.
	// Uses vertex, index and normals data from
	// https://github.com/daw42/glslcookbook/blob/master/ingredients/vbocube.cpp
	class CubePhong
		: public Renderable
	{
	public:
		// Parameters: program - GLSL program ID;
		//             camera - scene camera;
		//             position - position in the scene;
		//             side - side length;
		//             material - Phong material properties.
		CubePhong(GLuint program, Camera& camera, const glm::vec3& position, GLfloat side, const MaterialPhong& material);

		virtual ~CubePhong();

		// Create the primitive.
		virtual bool create() override;

		// Get the primitive type.
		virtual const wchar_t* getType() const override
		{
			return L"CubePhong";
		}

		virtual void render() const override;

	private:
		CubePhong(const CubePhong&) = delete;
		CubePhong& operator=(const CubePhong&) = delete;

		virtual std::vector<GLfloat> getVertices() const override;
		virtual std::vector<GLuint> getIndices() const override;
		virtual std::vector<GLfloat> getNormals() const override;

		void setMaterialProperties() const;

	private:
		const GLfloat m_side;

		const MaterialPhong m_material;

#if 0
		GLuint m_vao;
		GLuint m_vbo;

		GLuint m_index;          // index buffer
		GLsizei m_indexCount;    // number of indices

		GLuint m_normal;         // normal buffer
#endif
	};
}
