#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


Movable::Movable(void)
	: m_scaleFactor(1.0f)
{
}

void Movable::applyTranslation(const glm::vec3& diff)
{
	m_translation += diff;
}

void Movable::applyRotationX(GLfloat degrees)
{
	m_rotationDegrees.x += degrees;
}

void Movable::applyRotationY(GLfloat degrees)
{
	m_rotationDegrees.y += degrees;
}

void Movable::applyRotationZ(GLfloat degrees)
{
	m_rotationDegrees.z += degrees;
}

void Movable::applyScaling(GLfloat amount)
{
	m_scaleFactor += amount;
}
