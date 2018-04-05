#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


Cubemap::Cubemap(Camera& camera, GLfloat side, const std::string& textureDir)
	: m_camera(camera), m_cubeSide(side), m_textureDir(textureDir), 
	  m_vao{}, m_vbo{}, m_index{}, m_indexCount{}, m_texture{}
{
	// Initialize the program wrapper.

	const ShaderCollection shaders = {
		{ GL_VERTEX_SHADER,   "..\\..\\CommonLibOgl\\CommonLibOgl\\shaders\\cubemap.vert" },
		{ GL_FRAGMENT_SHADER, "..\\..\\CommonLibOgl\\CommonLibOgl\\shaders\\cubemap.frag" }
	};

	m_spProgram = std::make_unique<ProgramGLSL>(shaders);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	const GLfloat HalfSide = m_cubeSide / 2.0f;

	GLfloat vertices[] = {
		// Front
		-HalfSide, -HalfSide, HalfSide,
		HalfSide, -HalfSide, HalfSide,
		HalfSide, HalfSide, HalfSide,
		-HalfSide, HalfSide, HalfSide,
		// Right
		HalfSide, -HalfSide, HalfSide,
		HalfSide, -HalfSide, -HalfSide,
		HalfSide, HalfSide, -HalfSide,
		HalfSide, HalfSide, HalfSide,
		// Back
		-HalfSide, -HalfSide, -HalfSide,
		-HalfSide, HalfSide, -HalfSide,
		HalfSide, HalfSide, -HalfSide,
		HalfSide, -HalfSide, -HalfSide,
		// Left
		-HalfSide, -HalfSide, HalfSide,
		-HalfSide, HalfSide, HalfSide,
		-HalfSide, HalfSide, -HalfSide,
		-HalfSide, -HalfSide, -HalfSide,
		// Bottom
		-HalfSide, -HalfSide, HalfSide,
		-HalfSide, -HalfSide, -HalfSide,
		HalfSide, -HalfSide, -HalfSide,
		HalfSide, -HalfSide, HalfSide,
		// Top
		-HalfSide, HalfSide, HalfSide,
		HalfSide, HalfSide, HalfSide,
		HalfSide, HalfSide, -HalfSide,
		-HalfSide, HalfSide, -HalfSide
	};

	// Set up the vertex buffer.

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, _countof(vertices) * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	// Fill in the vertex position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint indices[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	// Set up the index buffer.

	m_indexCount = _countof(indices);

	glGenBuffers(1, &m_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _countof(indices) * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (!loadCubemapTextures())
	{
		assert(false); throw EXCEPTION(L"Failed to load skybox textures");
	}

	if (!m_spProgram->validate())
	{
		assert(false); throw EXCEPTION(L"GLSL program validation failed");
	}

	// Initialize matrices according to the camera state.
	updateViewMatrices();
}

Cubemap::~Cubemap()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	if (0 != m_texture)
	{
		glDeleteTextures(1, &m_texture);
	}

	if (0 != m_index)
	{
		glDeleteBuffers(1, &m_index);
	}

	if (0 != m_vbo)
	{
		glDeleteBuffers(1, &m_vbo);
	}

	if (0 != m_vao)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vao);
	}
}

bool Cubemap::loadCubemapTextures()
{
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

	std::string cubemapFiles[] = { "posx.png", "negx.png", "posy.png", "negy.png", "posz.png", "negz.png" };

	//const std::string folder = "data";

	auto del = [](unsigned char* pBuff) {
		SOIL_free_image_data(pBuff);
	};

	for (int i = 0; i < 6; ++i)
	{
		int width = {}, height = {};

		std::string filePath = m_textureDir + "/" + cubemapFiles[i];

		std::unique_ptr<unsigned char[], decltype(del)> spData(
			SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB));
		if (!spData)
		{
			std::wcerr << L"Failed to load texture " << filePath.c_str() << ": " << SOIL_last_result() << '\n';
			assert(false); return false;
		}

		glTexImage2D(targets[i], 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, spData.get());
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glUseProgram(m_spProgram->getProgram());

	glUniform1i(3, 0);

	glUseProgram(0);

	return true;
}

void Cubemap::updateViewMatrices() const
{
	assert(m_spProgram->getProgram());

	glUseProgram(m_spProgram->getProgram());

	glm::mat4 view = glm::mat4(glm::mat3(m_camera.getViewMatrix()));    // remove translation from the view matrix

	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(m_camera.getModelMatrix()));
	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(m_camera.getProjectionMatrix()));

	glUseProgram(0);
}

void Cubemap::translateCameraX(GLfloat diff)
{
	m_camera.translateX(diff);

	updateViewMatrices();
}

void Cubemap::translateCameraY(GLfloat diff)
{
	m_camera.translateY(diff);

	updateViewMatrices();
}

void Cubemap::translateCameraZ(GLfloat diff)
{
	m_camera.translateZ(diff);

	updateViewMatrices();
}

void Cubemap::rotateCameraX(GLfloat angleDegrees)
{
	m_camera.rotateX(angleDegrees);

	updateViewMatrices();
}

void Cubemap::rotateCameraY(GLfloat angleDegrees)
{
	m_camera.rotateY(angleDegrees);

	updateViewMatrices();
}

void Cubemap::rotateCameraZ(GLfloat angleDegrees)
{
	m_camera.rotateZ(angleDegrees);

	updateViewMatrices();
}

GLfloat Cubemap::getCameraScale() const
{
	return m_camera.getScale();
}

void Cubemap::scaleCamera(GLfloat amount)
{
	m_camera.scale(amount);

	updateViewMatrices();
}

void Cubemap::render() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_spProgram->getProgram());

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
