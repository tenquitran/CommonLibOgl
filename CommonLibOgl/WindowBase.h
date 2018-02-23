#pragma once


namespace CommonLibOgl
{
	// Base class for a window with OpenGL support.
	class WindowBase
	{
	public:
		// Parameters: hInstance - application instance;
		//             nCmdShow - controls how the window is to be shown;
		//             wndInfo - information about the window;
		//             openGLInfo - OpenGL settings.
		// Throws: Exception, std::bad_alloc
		WindowBase(HINSTANCE hInstance, int nCmdShow, const WindowInfo& wndInfo, const OpenGLInfo& openGLInfo);

		virtual ~WindowBase();

		int runMessageLoop();

	protected:
		// Camera controls.

		void translateCameraX(GLfloat diff);
		void translateCameraY(GLfloat diff);
		void translateCameraZ(GLfloat diff);

		void rotateCameraX(GLfloat angleDegrees);
		void rotateCameraY(GLfloat angleDegrees);
		void rotateCameraZ(GLfloat angleDegrees);

		void rotateCameraXY(GLfloat xAngleDegrees, GLfloat yAngleDegrees);

		GLfloat getCameraScale() const;

		void scaleCamera(GLfloat amount);

	protected:
		// The About box dialog procedure.
		static INT_PTR CALLBACK aboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		WindowBase(const WindowBase&) = delete;
		WindowBase& operator=(const WindowBase&) = delete;

		// Initialize the derived window.
		// Usually used to set up OpenGL scene: load objects, etc.
		virtual bool initialize() = 0;

		// Render the derived window contents.
		virtual void render() = 0;

		// The derived window procedure.
		// The base class window procedure processes WM_CREATE, WM_DESTROY, WM_SIZE and redirects WM_KEYDOWN here.
		virtual LRESULT windowProcDerived(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

		void resize(int clientWidth, int clientHeight);

		ATOM registerClass();

		BOOL initInstance(int nCmdShow);

		// Create and set up OpenGL context for the window.
		// Returns: true on success, false otherwise.
		bool setupOpenGlContext();

		// Debug callback for OpenGL-related errors.
		static void APIENTRY openGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
			GLsizei length, const GLchar* message, const void* param);

		// The main window procedure.
		static LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	protected:
		static const size_t MAX_LOADSTRING = 100;

		// Title bar text.
		TCHAR m_szTitle[MAX_LOADSTRING];

		// The window class name.
		TCHAR m_szWindowClass[MAX_LOADSTRING];

		// Application instance.
		HINSTANCE m_hInstance;

#if 0
		// Width and height of the window's client area (in pixels).
		int m_clientWidth;
		int m_clientHeight;
#else
		// Information about the window.
		WindowInfo m_wndInfo;

		// OpenGL settings.
		OpenGLInfo m_openGlInfo;
#endif

		// Window handle.
		HWND m_hWnd;

		// OpenGL rendering context for the main window.
		HGLRC m_hRC;

		// OpenGL camera.
		std::unique_ptr<Camera> m_spCamera;
	};
}
