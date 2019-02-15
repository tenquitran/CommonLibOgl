#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


ProgramGLSL::ProgramGLSL(const ShaderCollection& shaders)
	: m_shaderFiles(shaders)
{
	m_program = buildShaders(m_shaderFiles);

	if (0 == m_program)
	{
		throw EXCEPTION(L"Failed to build the program");
	}
}

ProgramGLSL::~ProgramGLSL()
{
	if (0 != m_program)
	{
		glDeleteProgram(m_program);
	}
}

GLuint ProgramGLSL::getProgram() const
{
	return m_program;
}

GLuint ProgramGLSL::buildShaders(const ShaderCollection& shaders)
{
	if (shaders.empty())
	{
		std::wcerr << L"No shader files to build\n";
		return 0;
	}

    // Create GLSL program.

	GLuint programId = glCreateProgram();
	if (0 == programId)
	{
		std::wcerr << L"glCreateProgram() failed\n";
		return 0;
	}

	// Process the list of shaders.

	std::vector<GLuint> shaderIds = {};

	for (const auto& itrShader : shaders)
	{   // Enumerate shader files.

		// Read the shader's source.

		const std::string fileName = itrShader.second;

		std::ifstream shaderFile(fileName);
		if (!shaderFile.good())
		{
			std::wcerr << L"Failed to load shader file \"" << fileName.c_str() << L"\"\n";
			return 0;
		}

		std::istreambuf_iterator<char> itrStream(shaderFile);

		std::string src;
		src.assign(itrStream, std::istreambuf_iterator<char>());

		shaderFile.close();

		// Create shader.

        GLuint shader = glCreateShader(itrShader.first);
		if (0 == shader)
		{
			std::wcerr << L"glCreateShader() failed\n";
			return 0;
		}

		shaderIds.push_back(shader);

		// Compile the shader and add it to the GLSL program.

		const GLchar *pBuff = static_cast<const GLchar*>(src.c_str());

		glShaderSource(shader, 1, (const GLchar**)&pBuff, nullptr);

		if (!compileShader(shader, fileName))
		{
			std::wcerr << L"Failed to compile shader \"" << fileName.c_str() << L"\"\n";
			return 0;
		}

		glAttachShader(programId, shader);

	}   // Enumerate shader files.

	// Link the program and check the results.

	glLinkProgram(programId);

	GLint resLink = GL_FALSE;
	glGetProgramiv(programId, GL_LINK_STATUS, &resLink);

	if (GL_TRUE != resLink)
	{
		GLint logLen = {};
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0)
		{
			std::unique_ptr<char[]> spLog = std::make_unique<char[]>(logLen);

			GLsizei written;
			glGetProgramInfoLog(programId, logLen, &written, spLog.get());

			std::wcerr << L"Failed to link the program: " << spLog.get() << '\n';

#if _DEBUG
			CAtlString msg;
			msg.Format(L"Failed to link the program:\n%S\n", spLog.get());
			::OutputDebugStringW(msg);
#endif
		}

		return 0;
	}

	// Clean up the shaders data.
	for (const auto& itr : shaderIds)
	{
		glDetachShader(programId, itr);
		glDeleteShader(itr);
	}

	return programId;
}

bool ProgramGLSL::compileShader(GLuint shader, const std::string& fileName)
{
	glCompileShader(shader);

	GLint res = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);

	if (GL_TRUE != res)
	{
		GLint logLen = {};
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0)
		{
			std::unique_ptr<char[]> spLog = std::make_unique<char[]>(logLen);

			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, spLog.get());

			std::wcerr << L"Failed to compile shader \"" << fileName.c_str() << L"\":\n" << spLog.get() << '\n';

#if _DEBUG
			CAtlString msg;
			msg.Format(L"Failed to compile shader %S:\n%S\n", fileName.c_str(), spLog.get());
			::OutputDebugStringW(msg);
#endif
		}

		return false;
	}

	return true;
}

bool ProgramGLSL::validate() const
{
	GLuint program = getProgram();

	glValidateProgram(program);

	GLint resValidation = GL_FALSE;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &resValidation);

	if (GL_TRUE != resValidation)
	{
		GLint logLen = {};
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0)
		{
			std::unique_ptr<char[]> spLog = std::make_unique<char[]>(logLen);

			GLsizei written;
			glGetProgramInfoLog(program, logLen, &written, spLog.get());

			std::wcerr << L"Failed to validate the program: " << spLog.get() << '\n';

#if _DEBUG
			CAtlString msg;
			msg.Format(L"Failed to validate the program:\n%S\n", spLog.get());
			::OutputDebugStringW(msg);
#endif
		}

		return false;
	}

	return true;
}
