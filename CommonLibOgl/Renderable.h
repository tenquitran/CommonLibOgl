#pragma once


namespace CommonLibOgl
{
	class Renderable
	{
	public:
		explicit Renderable(const Camera& camera)
			: m_camera(camera)
		{
		}

		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		virtual void updateViewMatrices() const = 0;

		virtual void render() const = 0;

	protected:
		const Camera& m_camera;
	};
}
