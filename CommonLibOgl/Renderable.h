#pragma once


namespace CommonLibOgl
{
	class Renderable
	{
	public:
		explicit Renderable(Camera& camera)
			: m_camera(camera)
		{
		}

		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		virtual void updateMatrices() const = 0;

		virtual void render() const = 0;

	protected:
		Camera& m_camera;
	};
}
