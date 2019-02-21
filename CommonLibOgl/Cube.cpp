#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


Cube::Cube(const glm::vec3& origin, GLfloat side)
	: Mesh(origin),
	  m_side(side)
{
	if (m_side < 0.0f)
	{
		ATLASSERT(FALSE); throw EXCEPTION(L"Negative side length");
	}

	// Set up the vertex buffer.

	const GLfloat HalfSide = m_side / 2;

	// A cube has 8 vertices.
	GLfloat vertices[] = {
		// front
		m_origin.x - HalfSide, m_origin.y - HalfSide, m_origin.z + HalfSide,
		m_origin.x + HalfSide, m_origin.y - HalfSide, m_origin.z + HalfSide,
		m_origin.x - HalfSide, m_origin.y + HalfSide, m_origin.z + HalfSide,
		m_origin.x + HalfSide, m_origin.y + HalfSide, m_origin.z + HalfSide,
		// back
		m_origin.x - HalfSide, m_origin.y - HalfSide, m_origin.z - HalfSide,
		m_origin.x + HalfSide, m_origin.y - HalfSide, m_origin.z - HalfSide,
		m_origin.x - HalfSide, m_origin.y + HalfSide, m_origin.z - HalfSide,
		m_origin.x + HalfSide, m_origin.y + HalfSide, m_origin.z - HalfSide };

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, _countof(vertices) * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	// Fill in the vertex position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	// Set up the index buffer.

	GLuint indices[] = { 
		// front
		0, 1, 3,
		0, 3, 2,
		// back
		4, 7, 5,
		4, 6, 7,
		// right
		0, 6, 4,
		0, 2, 6,
		// left
		1, 5, 7,
		1, 7, 3,
		// top
		2, 3, 7,
		2, 7, 6,
		// bottom
		0, 5, 1,
		0, 4, 5 };

	m_indexCount = _countof(indices);

	glGenBuffers(1, &m_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Set up the normal buffer.
	GLfloat normals[] = {
		// front
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		// back
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		// right
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		// left
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		// top
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		// bottom
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f };

	glGenBuffers(1, &m_normal);
	glBindBuffer(GL_ARRAY_BUFFER, m_normal);
	glBufferData(GL_ARRAY_BUFFER, _countof(normals) * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);

	// Fill in the normal attribute.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Cube::~Cube()
{
}

void Cube::render() const
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
