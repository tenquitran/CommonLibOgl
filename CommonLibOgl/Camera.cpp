#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////

#if 1
Camera::Camera(glm::vec3 position, GLfloat aspectRatio, GLfloat scaleFactor /*= 1.0f*/, GLfloat fieldOfView /*= 45.0f*/, 
	GLfloat frustumNear /*= 0.1f*/, GLfloat frustumFar /*= 1000.0f*/)
	: m_position(position), m_aspectRatio(aspectRatio), m_scaleFactor(scaleFactor), ScaleFactorMin(0.01f),
	  FieldOfView(fieldOfView), FrustumNear(frustumNear), FrustumFar(frustumFar)
	  //, m_yaw(-90.0f)
{
}

Camera::~Camera()
{
}

void Camera::resize(GLfloat aspectRatio)
{
	m_aspectRatio = aspectRatio;

	//updateProperties();
}

void Camera::translateX(GLfloat diff)
{
	m_position.x += diff;
}

void Camera::translateY(GLfloat diff)
{
	m_position.y += diff;
}

void Camera::translateZ(GLfloat diff)
{
	m_position.z += diff;
}

void Camera::rotateX(GLfloat degrees)
{
	m_rotationDegrees.x += degrees;
	//m_roll += degrees;
}

void Camera::rotateY(GLfloat degrees)
{
	m_rotationDegrees.y += degrees;
	//m_pitch += degrees;
}

void Camera::rotateZ(GLfloat degrees)
{
	m_rotationDegrees.z += degrees;
	//m_yaw += degrees;
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

glm::mat4 Camera::getModelViewMatrix() const
{
	// Apply translation. 
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position);

	// Apply rotation.
	model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.x), glm::vec3(1.0f, 0.0f, 0.0));    // X axis
	model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.y), glm::vec3(0.0f, 1.0f, 0.0));    // Y axis
	model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.z), glm::vec3(0.0f, 0.0f, 1.0));    // Z axis

#if 1
	// Apply scaling.
	model *= glm::scale(glm::mat4(1.0f), glm::vec3(m_scaleFactor));
#endif

	return glm::inverse(model);

	//return (view * model);

#if 0
	return glm::lookAt(
		m_position,
		//glm::vec3(0.0f, 0.0f, 0.45f),    // camera location
		glm::vec3(),                     // look towards this point
		glm::vec3(0.0f, 1.0f, 0.0f));    // the "up" direction
#endif

#if 0

#if 0
	glm::mat4 rotation = glm::yawPitchRoll(m_yaw, m_pitch, m_roll);

	glm::vec3 up = glm::vec3(rotation * glm::vec4(0, 1, 0, 0));

	glm::vec3 look = glm::vec3(rotation * glm::vec4(0, 0, 1, 0));

	return glm::lookAt(m_position, m_position + look, up);

	//return glm::lookAt(Position, Position + Front, Up);
#else
	glm::mat4 rotation = glm::yawPitchRoll(m_yaw, m_pitch, m_roll);
	//position += translation;
	//translation = glm::vec3();

	glm::vec3 look = glm::vec3(rotation * glm::vec4(0, 0, 1, 0));

	glm::vec3 target = m_position + look;

	glm::vec3 up = glm::vec3(rotation * glm::vec4(0, 1, 0, 0));

	glm::vec3 right = glm::cross(look, up);

	return glm::lookAt(m_position, target, up);
#endif

#endif
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return glm::perspective(glm::radians(FieldOfView), m_aspectRatio, FrustumNear, FrustumFar);
}

glm::mat4 Camera::getModelViewProjectionMatrix() const
{
	//m_projection * m_view * m_model;

	return (getProjectionMatrix() * getModelViewMatrix());
}

#if 0
void Camera::updateProperties()
{
	;

	// TODO: finish implementation
}
#endif

#else
Camera::Camera(glm::vec3 position, GLfloat aspectRatio, GLfloat scaleFactor /*= 1.0f*/,
	GLfloat fieldOfView /*= 45.0f*/, GLfloat frustumNear /*= 0.1f*/, GLfloat frustumFar /*= 1000.0f*/)
	: m_position(position), m_upWorld(0.0f, 1.0f, 0.0f), m_up(0.0f, 1.0f, 0.0f),
	  m_aspectRatio(aspectRatio), ScaleFactorMin(0.01f), m_scaleFactor(scaleFactor),
	  FieldOfView(fieldOfView), FrustumNear(frustumNear), FrustumFar(frustumFar),
	  m_yaw(-90.0f)
	  //, m_location(glm::vec3(0.0f, 0.0f, 2.5f))
{
#if 1
	m_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.5f),    // camera location
		glm::vec3(),                     // look towards this point
		glm::vec3(0.0f, 1.0f, 0.0f));    // the "up" direction
#endif

	updateMatrices();
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

void Camera::resize(GLfloat aspectRatio)
{
	m_aspectRatio = aspectRatio;

	updateMatrices();
}

#if 0
glm::vec3 Camera::getTranslation() const
{
	return m_translation;
}
#else
glm::vec3 Camera::getPosition() const
{
	return m_position;
}
#endif

void Camera::translateX(GLfloat diff)
{
	//m_translation.x += diff;

	m_position.x += diff;

	updateMatrices();
}

void Camera::translateY(GLfloat diff)
{
	//m_translation.y += diff;

	m_position.y += diff;

	updateMatrices();
}

void Camera::translateZ(GLfloat diff)
{
	//m_translation.z += diff;

	m_position.z += diff;

	updateMatrices();
}

void Camera::rotateX(GLfloat angleDegrees)
{
	//m_rotationDegrees.x += angleDegrees;
	m_yaw += angleDegrees;

	updateMatrices();
}

void Camera::rotateY(GLfloat angleDegrees)
{
	//m_rotationDegrees.y += angleDegrees;
	m_pitch += angleDegrees;

	updateMatrices();
}

#if 0
void Camera::rotateZ(GLfloat angleDegrees)
{
	m_rotationDegrees.z += angleDegrees;

	updateMVP();
}
#endif

void Camera::rotateXY(GLfloat xAngleDegrees, GLfloat yAngleDegrees)
{
#if 0
	m_rotationDegrees.x += xAngleDegrees;
	m_rotationDegrees.y += yAngleDegrees;
#else
	m_yaw   += xAngleDegrees;
	m_pitch += yAngleDegrees;
#endif

	updateMatrices();
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

	updateMatrices();
}

void Camera::updateMatrices()
{
	// WARNING: of all these matrices, only the view matrix can be initialized in the constructor.
	// The model matrix is often changed, and the projection matrix should take into account the current aspect ratio.

	// Projection matrix transforms coordinates from viewing space to screen space.
	m_projection = glm::perspective(glm::radians(FieldOfView), m_aspectRatio, FrustumNear, FrustumFar);

#if 0
	// Update the front, right and up vectors.

	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, m_upWorld));

	m_up = glm::normalize(glm::cross(m_right, m_front));

	// Update the view matrix.
	m_view = glm::lookAt(m_position, m_position + m_front, m_up);
#else

	// TODO: temp
#if 0
	m_view = glm::lookAt(m_position,    // camera location
		glm::vec3(),                     // look towards this point
		m_upWorld);    // the "up" direction
#endif

#endif

	// Apply translation. 
	// On doing this, reset the model matrix; otherwise, transformations will be accumulated.
	m_model = glm::translate(glm::mat4(1.0f), m_position);
	//m_model = glm::translate(glm::mat4(1.0f), m_translation);

	// Apply rotation.
#if 0
	m_model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.x), glm::vec3(1.0f, 0.0f, 0.0));    // X axis
	m_model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.y), glm::vec3(0.0f, 1.0f, 0.0));    // Y axis
	m_model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.z), glm::vec3(0.0f, 0.0f, 1.0));    // Z axis
#else
	m_model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_yaw), glm::vec3(1.0f, 0.0f, 0.0));    // X axis
	m_model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_pitch), glm::vec3(0.0f, 1.0f, 0.0));    // Y axis
	//m_model *= glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationDegrees.z), glm::vec3(0.0f, 0.0f, 1.0));    // Z axis
#endif

	// Apply scaling.
	m_model *= glm::scale(glm::mat4(1.0f), glm::vec3(m_scaleFactor));

#if 1
	//m_view = glm::inverse(m_model);
#endif

	m_mv = m_view * m_model;

	m_mvp = m_projection * m_view * m_model;
}
#endif
