#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


Plane::Plane(const glm::vec3& origin, GLfloat side, EPlaneDirection dir)
	: Mesh(origin),
	  m_side(side)
{
	if (EPlaneDirection::Undefined == dir)
	{
		ATLASSERT(FALSE); throw EXCEPTION(L"Invalid direction of the plane");
	}
	else if (m_side < 0.0f)
	{
		ATLASSERT(FALSE); throw EXCEPTION(L"Negative side length");
	}

	// Set up the vertex buffer.

	const GLfloat HalfSide = m_side / 2;

	std::vector<GLfloat> vertices;
	vertices.resize(12);

	switch (dir)
	{
	case EPlaneDirection::HorizontalYPositive:
	case EPlaneDirection::HorizontalYNegative:
		vertices[0]  = -HalfSide;
		vertices[1]  = m_origin.y;
		vertices[2]  = HalfSide;
		vertices[3]  = HalfSide;
		vertices[4]  = m_origin.y;
		vertices[5]  = HalfSide;
		vertices[6]  = -HalfSide;
		vertices[7]  = m_origin.y;
		vertices[8]  = -HalfSide;
		vertices[9]  = HalfSide;
		vertices[10] = m_origin.y;
		vertices[11] = -HalfSide;
		break;
	case EPlaneDirection::VerticalXPositive:
	case EPlaneDirection::VerticalXNegative:
		vertices[0]  = m_origin.x;
		vertices[1]  = -HalfSide;
		vertices[2]  = HalfSide;
		vertices[3]  = m_origin.x;
		vertices[4]  = -HalfSide;
		vertices[5]  = -HalfSide;
		vertices[6]  = m_origin.x;
		vertices[7]  = HalfSide;
		vertices[8]  = HalfSide;
		vertices[9]  = m_origin.x;
		vertices[10] = HalfSide;
		vertices[11] = -HalfSide;
		break;
	default:
		ATLASSERT(FALSE); break;
	}

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	// Fill in the vertex position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	// Set up the index buffer.

	GLuint indices[] = { 0, 1, 3, 0, 3, 2 };

	m_indexCount = _countof(indices);

	glGenBuffers(1, &m_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Set up the normal buffer.

	std::vector<GLfloat> normals(12, 0.0f);

	switch (dir)
	{
	case EPlaneDirection::HorizontalYPositive:
		for (size_t i = 1; i <= 10; i += 3)
		{
			normals[i] = 1.0f;
		}
		break;
	case EPlaneDirection::HorizontalYNegative:
		for (size_t i = 1; i <= 10; i += 3)
		{
			normals[i] = -1.0f;
		}
		break;
	case EPlaneDirection::VerticalXPositive:
		for (size_t i = {}; i <= 9; i += 3)
		{
			normals[i] = 1.0f;
		}
		break;
	case EPlaneDirection::VerticalXNegative:
		for (size_t i = {}; i <= 9; i += 3)
		{
			normals[i] = -1.0f;
		}
		break;
	default:
		ATLASSERT(FALSE); break;
	}

	glGenBuffers(1, &m_normal);
	glBindBuffer(GL_ARRAY_BUFFER, m_normal);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);

	// Fill in the normal attribute.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Plane::~Plane()
{
}

void Plane::render() const
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
