#pragma once


namespace CommonLibOgl
{
	// Mix-in class for the primitives that can be transformed.
	class Movable
	{
	public:
		Movable(void);

	protected:
		void applyTranslation(const glm::vec3& diff);

		void applyRotationX(GLfloat degrees);
		void applyRotationY(GLfloat degrees);
		void applyRotationZ(GLfloat degrees);

		void applyScaling(GLfloat amount);

	protected:
		glm::vec3 m_translation;

		glm::vec3 m_rotationDegrees;

		GLfloat m_scaleFactor;
	};
}
