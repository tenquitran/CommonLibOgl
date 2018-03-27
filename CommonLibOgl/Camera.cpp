#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


Camera::Camera(GLfloat aspectRatio, GLfloat scaleFactor /*= 1.0f*/, 
	GLfloat fieldOfView /*= 45.0f*/, GLfloat frustumNear /*= 0.1f*/, GLfloat frustumFar /*= 1000.0f*/)
	: m_aspectRatio(aspectRatio), ScaleFactorMin(0.01f), m_scaleFactor(scaleFactor),
	  FieldOfView(fieldOfView), FrustumNear(frustumNear), FrustumFar(frustumFar)
{
	m_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.5f),    // camera location
		glm::vec3(0.0f),                     // look towards this point
		glm::vec3(0.0f, 1.0f, 0.0f));        // the "up" direction

	updateMVP();
}

Camera::~Camera()
{
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return m_projection;
}

glm::mat4 Camera::getModelMatrix() const
{
	return m_model;
}

glm::mat4 Camera::getViewMatrix() const
{
	return m_view;
}

glm::mat4 Camera::getModelViewProjectionMatrix() const
{
	return m_mvp;
}

glm::mat4 Camera::getModelViewMatrix() const
{
	return m_mv;
}

void Camera::updateMVP()
{
	// WARNING: of all these matrices, only the view matrix can be initialized in the constructor.
	// The model matrix is often changed, and the projection matrix should take into account the current aspect ratio.

	// Projection matrix transforms coordinates from viewing space to screen space.
	m_projection = glm::perspective(glm::radians(FieldOfView), m_aspectRatio, FrustumNear, FrustumFar);

	// Reset the model matrix; otherwise, transformations will be accumulated.
	m_model = glm::mat4(1.0f);

	// Apply the user-defined translation component.
	m_model *= glm::translate(glm::mat4(1.0f), m_translation);

	// Apply the user-defined rotation components.
	m_model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.x), glm::vec3(1.0f, 0.0f, 0.0));    // X axis
	m_model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.y), glm::vec3(0.0f, 1.0f, 0.0));    // Y axis
	m_model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.z), glm::vec3(0.0f, 0.0f, 1.0));    // Z axis

	// Apply the scale factor.
	m_model *= glm::scale(glm::mat4(1.0f), glm::vec3(m_scaleFactor));

	m_mv = m_view * m_model;

	m_mvp = m_projection * m_view * m_model;
}

void Camera::resize(GLfloat aspectRatio)
{
	m_aspectRatio = aspectRatio;

	updateMVP();
}

glm::vec3 Camera::getTranslation() const
{
	return m_translation;
}

void Camera::translateX(GLfloat diff)
{
	m_translation.x += diff;

	updateMVP();
}

void Camera::translateY(GLfloat diff)
{
	m_translation.y += diff;

	updateMVP();
}

void Camera::translateZ(GLfloat diff)
{
	m_translation.z += diff;

	updateMVP();
}

void Camera::rotateX(GLfloat angleDegrees)
{
	m_rotationDegrees.x += angleDegrees;

	updateMVP();
}

void Camera::rotateY(GLfloat angleDegrees)
{
	m_rotationDegrees.y += angleDegrees;

	updateMVP();
}

void Camera::rotateZ(GLfloat angleDegrees)
{
	m_rotationDegrees.z += angleDegrees;

	updateMVP();
}

void Camera::rotateXY(GLfloat xAngleDegrees, GLfloat yAngleDegrees)
{
	m_rotationDegrees.x += xAngleDegrees;
	m_rotationDegrees.y += yAngleDegrees;

	updateMVP();
}

GLfloat Camera::getScale() const
{
	return m_scaleFactor;
}

void Camera::scale(GLfloat amount)
{
	m_scaleFactor += amount;

	// Prevent the camera flipping from zooming in to zooming out when the scale factor changes sign.
	if (m_scaleFactor < 0.0f)
	{
		m_scaleFactor = ScaleFactorMin;
		OutputDebugStringW(L"ZERO\n");
	}

	updateMVP();
}
