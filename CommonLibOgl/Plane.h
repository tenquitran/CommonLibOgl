#pragma once


namespace CommonLibOgl
{
	// Plane (either horizontal or vertical).
	class Plane
		: public Mesh
	{
	public:
		Plane(const glm::vec3& origin, GLfloat side, EPlaneDirection dir);

		virtual ~Plane();

		virtual void render() const override;

	private:
		const GLfloat m_side = { 1.0f };
	};
}
