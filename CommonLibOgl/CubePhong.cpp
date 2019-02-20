#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


CubePhong::CubePhong(GLuint program, Camera& camera, const glm::vec3& position, GLfloat side, const MaterialPhong& material)
	: Renderable(program, camera, position),
	  m_side(side), m_material(material)/*, m_vao{}, m_vbo{}, m_index{}, m_indexCount{}, m_normal{}*/
{
#if 0
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	const GLfloat HalfSide = m_side / 2.0f;

	GLfloat vertices[] = {
		// Front
		m_posInitial.x - HalfSide, m_posInitial.y - HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y - HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y + HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y + HalfSide, m_posInitial.z + HalfSide,
		// Right
		m_posInitial.x + HalfSide, m_posInitial.y - HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y - HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y + HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y + HalfSide, m_posInitial.z + HalfSide,
		// Back
		m_posInitial.x - HalfSide, m_posInitial.y - HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y + HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y + HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y - HalfSide, m_posInitial.z - HalfSide,
		// Left
		m_posInitial.x - HalfSide, m_posInitial.y - HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y + HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y + HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y - HalfSide, m_posInitial.z - HalfSide,
		// Bottom
		m_posInitial.x - HalfSide, m_posInitial.y - HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y - HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y - HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y - HalfSide, m_posInitial.z + HalfSide,
		// Top
		m_posInitial.x - HalfSide, m_posInitial.y + HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y + HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y + HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y + HalfSide, m_posInitial.z - HalfSide
	};

	// Set up the vertex buffer.

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, _countof(vertices) * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	// Fill in the vertex position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	setMaterialProperties();

	// Initialize matrices according to the camera state.
	updateMatrices();
#endif
}

CubePhong::~CubePhong()
{
#if 0
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
#endif
}

bool CubePhong::create()
{
	if (!initializeMesh())
	{
		std::wcerr << getType() << ": mesh initialization failed\n";
		return false;
	}

	setMaterialProperties();

	return true;
}

std::vector<GLfloat> CubePhong::getVertices() const
{
	const GLfloat HalfSide = m_side / 2.0f;

	return std::vector<GLfloat> {
		// Front
		m_posInitial.x - HalfSide, m_posInitial.y - HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y - HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y + HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y + HalfSide, m_posInitial.z + HalfSide,
		// Right
		m_posInitial.x + HalfSide, m_posInitial.y - HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y - HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y + HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y + HalfSide, m_posInitial.z + HalfSide,
		// Back
		m_posInitial.x - HalfSide, m_posInitial.y - HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y + HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y + HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y - HalfSide, m_posInitial.z - HalfSide,
		// Left
		m_posInitial.x - HalfSide, m_posInitial.y - HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y + HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y + HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y - HalfSide, m_posInitial.z - HalfSide,
		// Bottom
		m_posInitial.x - HalfSide, m_posInitial.y - HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y - HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y - HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y - HalfSide, m_posInitial.z + HalfSide,
		// Top
		m_posInitial.x - HalfSide, m_posInitial.y + HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y + HalfSide, m_posInitial.z + HalfSide,
		m_posInitial.x + HalfSide, m_posInitial.y + HalfSide, m_posInitial.z - HalfSide,
		m_posInitial.x - HalfSide, m_posInitial.y + HalfSide, m_posInitial.z - HalfSide };
}

std::vector<GLuint> CubePhong::getIndices() const
{
	return std::vector<GLuint> {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23 };
}

std::vector<GLfloat> CubePhong::getNormals() const
{
	return std::vector<GLfloat> {
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
		0.0f, 1.0f, 0.0f };
}

void CubePhong::setMaterialProperties() const
{
	glUseProgram(m_programId);

	GLuint mAmbient = glGetUniformLocation(m_programId, "Material.ambient");
	if (-1 != mAmbient)
	{
		glUniform3fv(mAmbient, 1, glm::value_ptr(m_material.m_ambientColor));
	}

	GLuint mDiffuse = glGetUniformLocation(m_programId, "Material.diffuse");
	if (-1 != mDiffuse)
	{
		glUniform3fv(mDiffuse, 1, glm::value_ptr(m_material.m_diffuseColor));
	}

	GLuint mSpecular = glGetUniformLocation(m_programId, "Material.specular");
	if (-1 != mSpecular)
	{
		glUniform3fv(mSpecular, 1, glm::value_ptr(m_material.m_specularColor));
	}

	GLuint mShininess = glGetUniformLocation(m_programId, "Material.shininess");
	if (-1 != mShininess)
	{
		glUniform1f(mShininess, m_material.m_shininess);
	}

	glUseProgram(0);
}

void CubePhong::render() const
{
	assert(m_programId);

	setMaterialProperties();

	updateMatrices();    // required for the independent movement

	glUseProgram(m_programId);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
