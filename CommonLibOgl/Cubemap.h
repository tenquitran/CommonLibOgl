#pragma once


namespace CommonLibOgl
{
	// TODO: fix and uncomment
#if 0
	// Cubemap.
	// Uses predefined texture file names: "posx.png", "negx.png", "posy.png", "negy.png", "posz.png", "negz.png"
	// Uses vertex, index and normals data from
	// https://github.com/daw42/glslcookbook/blob/master/ingredients/vbocube.cpp
	class Cubemap
		: public Renderable
	{
	public:
		// Parameters: camera - scene camera;
		//             side - cube side length;
		//             textureDir - full or partial path to directory with cubemap textures.
		Cubemap(Camera& camera, GLfloat side, const std::string& textureDir);

		virtual ~Cubemap();

		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		virtual void updateViewMatrices() const override;

		virtual void render() const override;

		//////////////////////////////////////////////////////////////////////////
		// Camera control.
		//////////////////////////////////////////////////////////////////////////

		void translateCameraX(GLfloat diff);
		void translateCameraY(GLfloat diff);
		void translateCameraZ(GLfloat diff);

		void rotateCameraX(GLfloat angleDegrees);
		void rotateCameraY(GLfloat angleDegrees);

#if 0
		void rotateCameraZ(GLfloat angleDegrees);
#endif

		GLfloat getCameraScale() const;

		void scaleCamera(GLfloat amount);

	private:
		Cubemap(const Cubemap&) = delete;
		Cubemap& operator=(const Cubemap&) = delete;

		bool loadCubemapTextures();

	private:
		Camera& m_camera;

		const GLfloat m_cubeSide;

		const std::string& m_textureDir;    // full or partial path to directory with cubemap textures

		std::unique_ptr<ProgramGLSL> m_spProgram;

		GLuint m_vao;
		GLuint m_vbo;

		GLuint m_index;          // index buffer
		GLsizei m_indexCount;    // number of indices

		GLuint m_texture;
	};
#endif
}
