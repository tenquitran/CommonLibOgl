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

	// TODO: implement
	ATLASSERT(FALSE);
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
