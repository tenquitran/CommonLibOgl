#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


Cube::Cube(const glm::vec3& origin, GLfloat side)
	: Mesh(origin),
	  m_side(side)
{
	// Set up the vertex buffer.

	//const GLfloat Height = glm::sqrt(glm::pow(m_side, 2) - glm::pow(m_side / 2, 2));

	const GLfloat HalfSide = m_side / 2;

	GLfloat vertices[] = {

#if 1
		// front
		m_origin.x - HalfSide, m_origin.y - HalfSide, m_origin.z + HalfSide,
		m_origin.x + HalfSide, m_origin.y - HalfSide, m_origin.z + HalfSide,
		m_origin.x - HalfSide, m_origin.y + HalfSide, m_origin.z + HalfSide,
		m_origin.x + HalfSide, m_origin.y + HalfSide, m_origin.z + HalfSide,

#else
		m_origin.x + HalfSide, m_origin.y - HalfSide, m_origin.z - HalfSide,
		m_origin.x + HalfSide, m_origin.y - HalfSide, m_origin.z + HalfSide,
		m_origin.x - HalfSide, m_origin.y - HalfSide, m_origin.z + HalfSide,
		m_origin.x - HalfSide, m_origin.y - HalfSide, m_origin.z - HalfSide,
		m_origin.x + HalfSide, m_origin.y + HalfSide, m_origin.z - HalfSide,
		m_origin.x + HalfSide, m_origin.y + HalfSide, m_origin.z + HalfSide,
		m_origin.x - HalfSide, m_origin.y + HalfSide, m_origin.z + HalfSide,
		m_origin.x - HalfSide, m_origin.y + HalfSide, m_origin.z - HalfSide 
#endif
	};

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, _countof(vertices) * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	// Fill in the vertex position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	// Set up the index buffer.

	GLuint indices[] = { 
#if 1
		// front
		0, 3, 1,
		0, 2, 3,

#else
		1, 3, 0,
		7, 5, 4,
		4, 1, 0,
		5, 2, 1,
		2, 7, 3,
		0, 7, 4,
		1, 2, 3,
		7, 6, 5,
		4, 5, 1,
		5, 6, 2,
		2, 6, 7,
		0, 3, 7 
#endif
	};

	m_indexCount = _countof(indices);

	glGenBuffers(1, &m_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Set up the normal buffer.
	GLfloat normals[] = {
#if 1
		// front
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
#else
		 0.0f, -1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 0.0f,  0.0f,  1.0f,
		-1.0f,  0.0f,  0.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 0.0f,  0.0f,  1.0f,
		-1.0f,  0.0f,  0.0f,
		 0.0f,  0.0f, -1.0f 
#endif
	};

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
