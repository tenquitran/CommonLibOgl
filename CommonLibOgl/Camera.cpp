#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


Camera::Camera(glm::vec3 position, GLfloat aspectRatio, GLfloat scaleFactor /*= 1.0f*/,
	GLfloat fieldOfView /*= 45.0f*/, GLfloat frustumNear /*= 0.1f*/, GLfloat frustumFar /*= 1000.0f*/)
	: InitialPosition(position), Up(0.0f, 1.0f, 0.0f), m_aspectRatio(aspectRatio), 
	  ScaleFactorMin(0.01f), m_scaleFactor(scaleFactor), 
	  FieldOfView(fieldOfView), FrustumNear(frustumNear), FrustumFar(frustumFar)
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return glm::perspective(glm::radians(FieldOfView), m_aspectRatio, FrustumNear, FrustumFar);
}

glm::mat4 Camera::getModelMatrix() const
{
	// Apply translation. 
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translation);

	// Apply rotation.
	model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.x), glm::vec3(1.0f, 0.0f, 0.0));    // X axis
	model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.y), glm::vec3(0.0f, 1.0f, 0.0));    // Y axis
	model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.z), glm::vec3(0.0f, 0.0f, 1.0));    // Z axis

	// Apply scaling.
	model *= glm::scale(glm::mat4(1.0f), glm::vec3(m_scaleFactor));

	return model;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(InitialPosition,    // camera location
		Front,                             // look towards this point
		Up);                               // the "up" direction
}

glm::mat4 Camera::getModelViewProjectionMatrix() const
{
	return (getProjectionMatrix() * getViewMatrix() * getModelMatrix());
}

glm::mat4 Camera::getModelViewMatrix() const
{
	return (getViewMatrix() * getModelMatrix());
}

void Camera::resize(GLfloat aspectRatio)
{
	m_aspectRatio = aspectRatio;
}

glm::vec3 Camera::getPosition() const
{
	return (InitialPosition + m_translation);
}

void Camera::translateX(GLfloat diff)
{
	m_translation.x += diff;
}

void Camera::translateY(GLfloat diff)
{
	m_translation.y += diff;
}

void Camera::translateZ(GLfloat diff)
{
	m_translation.z += diff;
}

void Camera::rotateX(GLfloat degrees)
{
	m_rotationDegrees.x += degrees;
}

void Camera::rotateY(GLfloat degrees)
{
	m_rotationDegrees.y += degrees;
}

void Camera::rotateZ(GLfloat degrees)
{
	m_rotationDegrees.z += degrees;
}

void Camera::rotateXY(GLfloat xDegrees, GLfloat yDegrees)
{
	m_rotationDegrees.x += xDegrees;
	m_rotationDegrees.y += yDegrees;
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
#if _DEBUG
		OutputDebugStringW(L"ZERO\n");
#endif
#if _DEBUG
	}
	else
	{
		CAtlString msg;
		msg.Format(L"Camera scale: %0.3f\n", m_scaleFactor);
		OutputDebugStringW(msg.GetString());
	}
#endif
}
