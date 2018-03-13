#pragma once


namespace CommonLibOgl
{
	class Renderable
	{
	public:
		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		virtual void updateViewMatrices() const = 0;

		virtual void render() const = 0;
	};
}
