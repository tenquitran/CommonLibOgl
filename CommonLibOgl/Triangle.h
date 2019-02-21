#pragma once


namespace CommonLibOgl
{
	class Triangle
		: public Mesh
	{
	public:
		Triangle(const glm::vec3& origin, GLfloat side);

		virtual ~Triangle();

		virtual void render() const override;

	private:
		const GLfloat m_side = { 1.0f };
	};
}
