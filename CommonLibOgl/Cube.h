#pragma once


namespace CommonLibOgl
{
	class Cube
		: public Mesh
	{
	public:
		Cube(const glm::vec3& origin, GLfloat side);

		virtual ~Cube();

		virtual void render() const override;

	private:
		const GLfloat m_side = { 1.0f };
	};
}
