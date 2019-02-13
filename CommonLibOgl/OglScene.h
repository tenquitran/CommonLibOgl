#pragma once


namespace CommonLibOgl
{
    // Base class for OpenGL scene.
    class OglScene
    {
    public:
        OglScene();

        virtual ~OglScene();

#pragma region ToImplement

        // Initialize the derived scene.
        virtual bool initialize() = 0;

        // Update OpenGL uniforms such as ModelView matrix (can be empty).
        virtual void updateUniforms(const std::unique_ptr<Camera>& spCamera) const = 0;

        // Render the derived scene.
        virtual void render() const = 0;

#pragma endregion ToImplement

        // Get GLSL program ID.
        // Returns: non-zero value on success.
        GLuint getGlslProgramId() const;

        // Initialize the base scene.
        // Parameters: aspectRatio - aspect ratio of the window's client area;
        //             openGlInfo - OpenGL settings;
        //             shaders - shaders to use. Example:
        //
        // const ShaderCollection shaders = {
        //    { GL_VERTEX_SHADER,   "shaders\\terrain.vert" },
        //    { GL_FRAGMENT_SHADER, "shaders\\terrain.frag" }
        // };
        //
        //             cameraOffset - (optional) initial camera offset.
        bool initializeBase(GLfloat aspectRatio, const CommonLibOgl::OpenGLInfo& openGlInfo, 
            const ShaderCollection& shaders, const glm::vec3& cameraOffset = {});

        void resize(GLfloat aspectRatio);

        void renderBase() const;

        //////////////////////////////////////////////////////////////////////////
        // Camera control.
        //////////////////////////////////////////////////////////////////////////

        void translateCameraX(GLfloat diff);
        void translateCameraY(GLfloat diff);
        void translateCameraZ(GLfloat diff);

        void rotateCameraX(GLfloat angleDegrees);
        void rotateCameraY(GLfloat angleDegrees);
        void rotateCameraZ(GLfloat angleDegrees);

        void rotateCameraXY(GLfloat xAngleDegrees, GLfloat yAngleDegrees);

        GLfloat getCameraScale() const;

        void scaleCamera(GLfloat amount);

    private:
        std::unique_ptr<CommonLibOgl::ProgramGLSL> m_spProgram;

        // OpenGL camera.
        std::unique_ptr<CommonLibOgl::Camera> m_spCamera;
    };
}
