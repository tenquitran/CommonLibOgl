#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


Renderable::Renderable(GLuint program, Camera& camera, const glm::vec3& position)
	: m_programId(program), m_camera(camera), m_posInitial(position, 0.0f), m_scaleFactor(1.0f),
	  m_vao{}, m_vbo{}, m_index{}, m_indexCount{}, m_normal{}
{
	if (!m_programId)
	{
		assert(false); throw EXCEPTION(L"Invalid GLSL program ID");
	}
}

Renderable::Renderable(Camera& camera, const glm::vec3& position)
	: m_programId{}, m_camera(camera), m_posInitial(position, 0.0f), m_scaleFactor(1.0f), 
	  m_vao{}, m_vbo{}, m_index{}, m_indexCount{}, m_normal{}
{
}

Renderable::~Renderable()
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

bool Renderable::initializeMesh()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Set up the vertex buffer.

	std::vector<GLfloat> vertices = getVertices();
	if (vertices.empty())
	{
		std::wcerr << "No vertex coordinates\n";
		assert(false); return false;
	}

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	// Fill in the vertex position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	// Set up the index buffer.

#if 1
	std::vector<GLuint> indices = getIndices();
	if (indices.empty())
	{
		std::wcerr << "No indices\n";
		assert(false); return false;
	}
#else
	GLuint indices[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};
#endif

	m_indexCount = indices.size();

	glGenBuffers(1, &m_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Set up the normal buffer.

#if 1
	std::vector<GLfloat> normals = getNormals();
	if (normals.empty())
	{
		std::wcerr << "No normals\n";
		assert(false); return false;
	}
#else
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
#endif

	glGenBuffers(1, &m_normal);
	glBindBuffer(GL_ARRAY_BUFFER, m_normal);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);

	// Fill in the normal attribute.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//createDerived();

	// Initialize matrices according to the camera state.
	updateMatrices();

    return true;
}

void Renderable::translate(const glm::vec3& diff)
{
	m_translation += diff;

	updateMatrices();
}

glm::vec4 Renderable::getPosition() const
{
	return (m_posInitial + glm::vec4(m_translation, 0.0f));
}

void Renderable::rotateX(GLfloat degrees)
{
	m_rotationDegrees.x += degrees;

	updateMatrices();
}

void Renderable::rotateY(GLfloat degrees)
{
	m_rotationDegrees.y += degrees;

	updateMatrices();
}

void Renderable::rotateZ(GLfloat degrees)
{
	m_rotationDegrees.z += degrees;

	updateMatrices();
}

void Renderable::scale(GLfloat amount)
{
	m_scaleFactor += amount;

	updateMatrices();
}

void Renderable::updateMatrices() const
{
	assert(m_programId);

	glUseProgram(m_programId);

	glm::mat4 proj = m_camera.getProjectionMatrix();
	glm::mat4 view = m_camera.getViewMatrix();
	glm::mat4 model = m_camera.getModelMatrix();

	// Apply translation.
	model *= glm::translate(glm::mat4(1.0f), m_translation);

	// Apply rotation.
	model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.x), glm::vec3(1.0f, 0.0f, 0.0));    // X axis
	model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.y), glm::vec3(0.0f, 1.0f, 0.0));    // Y axis
	model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.z), glm::vec3(0.0f, 0.0f, 1.0));    // Z axis

	// Apply scaling.
	model *= glm::scale(glm::mat4(1.0f), glm::vec3(m_scaleFactor));

	glm::mat4 mv = view * model;

	glm::mat4 mvp = proj * view * model;

	assert(m_programId);

	glUseProgram(m_programId);

	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));

	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(mv));

	glm::mat3 normal = glm::mat3(glm::transpose(glm::inverse(mv)));
	//glm::mat3 normal = glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2]));

	glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normal));

	glUseProgram(0);
}
