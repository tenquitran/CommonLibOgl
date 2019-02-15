#include "stdafx.h"

#if 0
//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


OglScene::OglScene()
{
}

OglScene::~OglScene()
{
}

GLuint OglScene::getGlslProgramId() const
{
    if (!m_spProgram)
    {
        ATLASSERT(FALSE); return 0;
    }

    return m_spProgram->getProgram();
}

bool OglScene::initializeBase(GLfloat aspectRatio, const OpenGLInfo& openGlInfo, const ShaderCollection& shaders, 
    const glm::vec3& cameraOffset /*= {}*/)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_MULTISAMPLE);

    glClearColor(0.8f, 0.93f, 0.96f, 1.0f);    // very light blue

    // Initial scale factor for the camera.
    const GLfloat CameraScaleFactor = 1.0f;

    glm::vec3 cameraPosition = { 0.0f, 0.0f, -5.0f };

    m_spCamera = std::make_unique<Camera>(cameraPosition, aspectRatio, CameraScaleFactor,
        openGlInfo.FieldOfView, openGlInfo.FrustumNear, openGlInfo.FrustumFar);

    // Use the initial camera offset.
    m_spCamera->translateX(cameraOffset.x);
    m_spCamera->translateY(cameraOffset.y);
    m_spCamera->translateZ(cameraOffset.z);

    // Initialize the program wrapper.

    m_spProgram = std::make_unique<ProgramGLSL>(shaders);

    if (!initializeDerived())
    {
        std::wcerr << L"OglScene: derived class initialization failed\n";
        ATLASSERT(FALSE); return false;
    }

    // TODO: set uniforms if required
#if 0
    glUseProgram(m_spProgram->getProgram());

    ;

    glUseProgram(0);
#endif

    if (!m_spProgram->validate())
    {
        std::wcerr << L"GLSL program validation failed\n";
        ATLASSERT(FALSE); return false;
    }

    updateUniformsDerived(m_spCamera);

    return true;
}

void OglScene::translateCameraX(GLfloat diff)
{
    m_spCamera->translateX(diff);

    updateUniformsDerived(m_spCamera);
}

void OglScene::translateCameraY(GLfloat diff)
{
    m_spCamera->translateY(diff);

    updateUniformsDerived(m_spCamera);
}

void OglScene::translateCameraZ(GLfloat diff)
{
    m_spCamera->translateZ(diff);

    updateUniformsDerived(m_spCamera);
}

void OglScene::rotateCameraX(GLfloat angleDegrees)
{
    m_spCamera->rotateX(angleDegrees);

    updateUniformsDerived(m_spCamera);
}

void OglScene::rotateCameraY(GLfloat angleDegrees)
{
    m_spCamera->rotateY(angleDegrees);

    updateUniformsDerived(m_spCamera);
}

void OglScene::rotateCameraZ(GLfloat angleDegrees)
{
    m_spCamera->rotateZ(angleDegrees);

    updateUniformsDerived(m_spCamera);
}

void OglScene::rotateCameraXY(GLfloat xAngleDegrees, GLfloat yAngleDegrees)
{
    m_spCamera->rotateXY(xAngleDegrees, yAngleDegrees);

    updateUniformsDerived(m_spCamera);
}

GLfloat OglScene::getCameraScale() const
{
    return m_spCamera->getScale();
}

void OglScene::scaleCamera(GLfloat amount)
{
    m_spCamera->scale(amount);

    updateUniformsDerived(m_spCamera);
}

void OglScene::resize(GLfloat aspectRatio)
{
    m_spCamera->resize(aspectRatio);

    updateUniformsDerived(m_spCamera);
}

void OglScene::renderBase() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render();
}
#endif
