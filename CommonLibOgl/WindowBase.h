#pragma once


namespace CommonLibOgl
{
	// Base class for a window with OpenGL support.
	class WindowBase
	{
	public:
		// Parameters: hInstance - application instance;
		//             wndInfo - information about the window;
		//             openGLInfo - OpenGL settings.
		// Throws: Exception, std::bad_alloc
		WindowBase(HINSTANCE hInstance, const WindowInfo& wndInfo, const OpenGLInfo& openGLInfo);

		virtual ~WindowBase();

		// Run the window's message loop.
		// Parameters: nCmdShow - controls how the window is to be shown.
		int runMessageLoop(int nCmdShow);

	protected:
		// The About box dialog procedure.
		static INT_PTR CALLBACK aboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		WindowBase(const WindowBase&) = delete;
		WindowBase& operator=(const WindowBase&) = delete;

		void resize(int clientWidth, int clientHeight);

		// Notify the derived window about resizing.
		virtual void onResizeDerived(int clientWidth, int clientHeight) = 0;

		// Initialize the derived window.
		// Usually used to set up OpenGL scene: load objects, etc.
		virtual bool initialize() = 0;

		// Render the derived window contents.
		virtual void render() const = 0;

		// The derived window procedure.
		// The base class window procedure processes WM_CREATE, WM_DESTROY, WM_SIZE and redirects WM_KEYDOWN here.
		virtual LRESULT windowProcDerived(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

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

		// Information about the window.
		WindowInfo m_wndInfo;

		// OpenGL settings.
		OpenGLInfo m_openGlInfo;

		// Window handle.
		HWND m_hWnd;

		// OpenGL rendering context for the main window.
		HGLRC m_hRC;

		HWND m_hWndTemporary;
	};
}
