#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


LightSourceVisible::LightSourceVisible(Camera& camera, const glm::vec4& position, GLfloat side, const glm::vec3& color)
	: Renderable(camera, position),
	  m_side(side), m_colorEmissive(color), m_vao{}, m_vbo{}, m_index{}, m_indexCount{}
{
#if 0
	const ShaderCollection shaders = {
		{ GL_VERTEX_SHADER,   "..\\..\\CommonLibOgl\\CommonLibOgl\\shaders\\lightSourceVisible.vert" },
		{ GL_FRAGMENT_SHADER, "..\\..\\CommonLibOgl\\CommonLibOgl\\shaders\\lightSourceVisible.frag" }
	};

	m_spProgram = std::make_unique<ProgramGLSL>(shaders);

	m_programId = m_spProgram->getProgram();

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
	updateMatrices();
#endif
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

bool LightSourceVisible::create()
{
	const ShaderCollection shaders = {
		{ GL_VERTEX_SHADER,   "..\\..\\CommonLibOgl\\CommonLibOgl\\shaders\\lightSourceVisible.vert" },
		{ GL_FRAGMENT_SHADER, "..\\..\\CommonLibOgl\\CommonLibOgl\\shaders\\lightSourceVisible.frag" }
	};

	m_spProgram = std::make_unique<ProgramGLSL>(shaders);

	m_programId = m_spProgram->getProgram();

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

	return true;
}

void LightSourceVisible::updateMatrices() const
{
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

	glm::mat4 mvp = proj * view * model;

	GLuint program = m_spProgram->getProgram();
	assert(program);

	glUseProgram(program);

	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));
	//glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(m_camera.getModelViewProjectinMatrix()));

	glUseProgram(0);
}

void LightSourceVisible::render() const
{
	assert(m_spProgram);

	updateMatrices();    // required for the independent movement

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
