#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


Renderable::Renderable(GLuint program, Camera& camera, const glm::vec3& position)
	: m_programId(program), m_camera(camera), m_posInitial(position, 0.0f), m_scaleFactor(1.0f)
{
	if (!m_programId)
	{
		assert(false); throw EXCEPTION(L"Invalid GLSL program ID");
	}
}

Renderable::Renderable(Camera& camera, const glm::vec3& position)
	: m_programId{}, m_camera(camera), m_posInitial(position, 0.0f), m_scaleFactor(1.0f)
{
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
