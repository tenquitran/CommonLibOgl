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

        // It's better to validate the program after setting all texture sampler uniforms 
		// (usually, at the end of the OpenGL scene initialization).
		// Otherwise, on using cubemap with some other texture, program validation will fail with this error:
		//
		// Validation failed! - Different sampler types for same sample texture unit in fragment shader.
		bool validate() const;

	private:
		ProgramGLSL(const ProgramGLSL&);
		ProgramGLSL& operator=(const ProgramGLSL&);

		// Build shaders.
		// Parameters: shaders - list of shaders to build (key: shader type; value: shader file name or relative path).
		// Returns: program handle (a positive integer) on success, zero otherwise.
		// Throws: std::bad_alloc
		GLuint buildShaders(const ShaderCollection& shaders);

		// Compile the shader.
		// Parameters: shader - OpenGL shader object;
		//             fileName - shader file name (for logging).
        // Returns: true on success, false otherwise.
		bool compileShader(GLuint shader, const std::string& fileName);

	private:
		// GLSL program.
        GLuint m_program = {};

		// Data on shader files to load and build.
		const ShaderCollection m_shaderFiles;
	};
}
