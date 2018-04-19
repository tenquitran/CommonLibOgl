#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


LightSourceVisible::LightSourceVisible(const Camera& camera, float cubeSide, const glm::vec3& color)
	: Renderable(camera),
	//m_camera(camera), 
	m_cubeSide(cubeSide), m_colorEmissive(color), m_vao{}, m_vbo{}, m_index{}, m_indexCount{}
{
	const ShaderCollection shaders = {
		{ GL_VERTEX_SHADER,   "shaders\\lightSourceVisible.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\lightSourceVisible.frag" }
	};

	m_spProgram = std::make_unique<ProgramGLSL>(shaders);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	const float HalfSide = m_cubeSide / 2.0f;

	float vertices[] = {
		// Front
		-HalfSide, -HalfSide, HalfSide,
		 HalfSide, -HalfSide, HalfSide,
		 HalfSide,  HalfSide, HalfSide,
		-HalfSide,  HalfSide, HalfSide,
		// Right
		HalfSide, -HalfSide, HalfSide,
		HalfSide, -HalfSide, -HalfSide,
		HalfSide, HalfSide, -HalfSide,
		HalfSide, HalfSide, HalfSide,
		// Back
		-HalfSide, -HalfSide, -HalfSide,
		-HalfSide, HalfSide, -HalfSide,
		HalfSide, HalfSide, -HalfSide,
		HalfSide, -HalfSide, -HalfSide,
		// Left
		-HalfSide, -HalfSide, HalfSide,
		-HalfSide, HalfSide, HalfSide,
		-HalfSide, HalfSide, -HalfSide,
		-HalfSide, -HalfSide, -HalfSide,
		// Bottom
		-HalfSide, -HalfSide, HalfSide,
		-HalfSide, -HalfSide, -HalfSide,
		HalfSide, -HalfSide, -HalfSide,
		HalfSide, -HalfSide, HalfSide,
		// Top
		-HalfSide, HalfSide, HalfSide,
		HalfSide, HalfSide, HalfSide,
		HalfSide, HalfSide, -HalfSide,
		-HalfSide, HalfSide, -HalfSide
	};

	// Set up the vertex buffer.

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, _countof(vertices) * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	// Fill in the vertex position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint indices[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	// Set up the index buffer.

	m_indexCount = _countof(indices);

	glGenBuffers(1, &m_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _countof(indices) * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(m_spProgram->getProgram());

	// Set the light color uniform.
	glUniform3fv(1, 1, glm::value_ptr(m_colorEmissive));

	glUseProgram(0);

	if (!m_spProgram->validate())
	{
		assert(false); throw EXCEPTION(L"GLSL program validation failed");
	}

	// Initialize matrices according to the camera state.
	updateViewMatrices();
}

LightSourceVisible::~LightSourceVisible()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (0 != m_index)
	{
		glDeleteBuffers(1, &m_index);
	}

	if (0 != m_vbo)
	{
		glDeleteBuffers(1, &m_vbo);
	}

	if (0 != m_vao)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vao);
	}
}

void LightSourceVisible::updateViewMatrices() const
{
	assert(m_spProgram);

	glUseProgram(m_spProgram->getProgram());

	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(m_camera.getModelViewProjectionMatrix()));

	glUseProgram(0);
}

void LightSourceVisible::render() const
{
	// TODO: move?
	updateViewMatrices();

	assert(m_spProgram);

	glUseProgram(m_spProgram->getProgram());
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
