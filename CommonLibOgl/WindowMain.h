#pragma once


namespace CommonLibOgl
{
    // Main window of the application.
    // Note that it doesn't derive from WindowBase.
    class WindowMain
    {
    public:
        // Parameters: hInstance - application instance;
        //             wndInfo - window settings;
        //             openGLInfo - OpenGL settings.
        // Throws: Exception, std::bad_alloc
        WindowMain(HINSTANCE hInstance, const WindowInfo& wndInfo, const OpenGLInfo& openGLInfo, std::weak_ptr<OglScene>& scene);

        virtual ~WindowMain();

        // Get GLSL program ID.
        // Returns: non-zero value on success.
        GLuint getGlslProgramId() const;

        // Run the window's message loop.
        // Parameters: nCmdShow - controls how the window is to be shown;
        //             shaders - shaders to use. Example:
        //
        // const ShaderCollection shaders = {
        //    { GL_VERTEX_SHADER,   "shaders\\terrain.vert" },
        //    { GL_FRAGMENT_SHADER, "shaders\\terrain.frag" }
        // };
        //
        //             cameraOffset - (optional) initial camera offset.
        int runMessageLoop(int nCmdShow, const ShaderCollection& shaders, const glm::vec3& cameraOffset = {});

    protected:
        // The About box dialog procedure.
        static INT_PTR CALLBACK aboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    private:
        WindowMain(const WindowMain&) = delete;
        WindowMain& operator=(const WindowMain&) = delete;

        ATOM registerClass();

        BOOL initInstance(int nCmdShow);

        // Create and set up OpenGL context for the window.
        // Returns: true on success, false otherwise.
        bool setupOpenGlContext();

        // Debug callback for OpenGL-related errors.
        static void APIENTRY openGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar* message, const void* param);

        // The window procedure.
        static LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        void resize(int clientWidth, int clientHeight);

        // Render the window contents.
        void render() const;

    private:
        static const size_t MAX_LOADSTRING = 100;

        // Title bar text.
        TCHAR m_szTitle[MAX_LOADSTRING];

        // Window class name.
        TCHAR m_szWindowClass[MAX_LOADSTRING];

        // Application instance.
        const HINSTANCE m_hInstance = { nullptr };

        // Information about the window.
        WindowInfo m_wndInfo;

        // OpenGL settings.
        const OpenGLInfo m_openGlInfo;

        // Window handle.
        HWND m_hWnd = { nullptr };

        // OpenGL rendering context for the main window.
        HGLRC m_hRC = { nullptr };

        HWND m_hWndTemporary = { nullptr };

        // OpenGL scene.
        std::shared_ptr<OglScene> m_spScene;
    };
}
