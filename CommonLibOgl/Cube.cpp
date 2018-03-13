#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


Cube::Cube(GLuint program, const Camera& camera, float cubeSide, const MaterialPhong& material)
	: m_program(program), m_camera(camera), m_cubeSide(cubeSide), m_material(material), 
	  m_vao{}, m_vbo{}, m_index{}, m_indexCount{}, m_normal{}
{
	if (!m_program)
	{
		assert(false); throw EXCEPTION(L"Invalid GLSL program");
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	const float HalfSide = m_cubeSide / 2.0f;

	float vertices[] = {
		// Front
		-HalfSide, -HalfSide, HalfSide,
		HalfSide, -HalfSide, HalfSide,
		HalfSide, HalfSide, HalfSide,
		-HalfSide, HalfSide, HalfSide,
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

	GLfloat normals[] = {
		// Front
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		// Right
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		// Back
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		// Left
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		// Bottom
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		// Top
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// Set up the normal buffer.

	glGenBuffers(1, &m_normal);
	glBindBuffer(GL_ARRAY_BUFFER, m_normal);
	glBufferData(GL_ARRAY_BUFFER, _countof(normals) * sizeof(normals[0]), normals, GL_STATIC_DRAW);

	// Fill in the normal attribute.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

	glUseProgram(m_program);

	// TODO: set uniforms

	glUseProgram(0);

	// Initialize matrices according to the camera state.
	updateViewMatrices();
}

Cube::~Cube()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (0 != m_normal)
	{
		glDeleteBuffers(1, &m_normal);
	}

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

void Cube::updateViewMatrices() const
{
	assert(m_program);

	glUseProgram(m_program);

	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(m_camera.getModelViewProjectionMatrix()));

	glUseProgram(0);
}

void Cube::render() const
{
	assert(m_program);

	glUseProgram(m_program);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}