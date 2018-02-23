#pragma once


namespace CommonLibOgl
{
	// Collection of shaders.
	// Key: shader type; value: partial path to the shader file.
	typedef std::map<GLenum, std::string> ShaderCollection;


	// GLSL object; encapsulates shaders.
	class ProgramGLSL
	{
	public:
		// Parameters: shaders - description of the shaders to load and build.
		// Throws: Exception, std::bad_alloc
		explicit ProgramGLSL(const ShaderCollection& shaders);

		virtual ~ProgramGLSL();

		GLuint getProgram() const;

	private:
		ProgramGLSL(const ProgramGLSL&);
		ProgramGLSL& operator=(const ProgramGLSL&);

		// Build the shaders.
		// Parameters: shaders - list of shaders to build (key: shader type; value: shader file name or relative path).
		// Returns: program handle (a positive integer) on success, zero otherwise.
		// Throws: std::bad_alloc
		GLuint buildShaders(const ShaderCollection& shaders);

		// Compile the shader.
		// Parameters: shader - OpenGL shader object;
		//             fileName - shader file name (for logging).
		bool compileShader(GLuint shader, const std::string& fileName);

	private:
		// GLSL program.
		GLuint m_program;

		// Data on shader files to load and build.
		const ShaderCollection m_shaderFiles;
	};
}
