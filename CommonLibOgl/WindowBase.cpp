#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


WindowBase::WindowBase(HINSTANCE hInstance, int nCmdShow, const WindowInfo& wndInfo, const OpenGLInfo& openGLInfo)
	: m_hInstance(hInstance), m_wndInfo(wndInfo), m_openGlInfo(openGLInfo), m_hWnd(nullptr), m_hRC(nullptr)
{
	if (!m_hInstance)
	{
		assert(false); throw EXCEPTION(L"Application instance is NULL");
	}
	else if (   m_wndInfo.m_clientWidth  < 1
		     || m_wndInfo.m_clientHeight < 1)
	{
		assert(false); throw EXCEPTION_FMT(L"Invalid window size: (%d, %d)", m_wndInfo.m_clientWidth, m_wndInfo.m_clientHeight);
	}

	// Initialize global strings.
	LoadString(hInstance, m_wndInfo.m_titleId, m_szTitle, MAX_LOADSTRING);
	LoadString(hInstance, m_wndInfo.m_wndClassId, m_szWindowClass, MAX_LOADSTRING);

	if (!registerClass())
	{
		assert(false); throw EXCEPTION(L"Failed to register windows class");
	}

	// Perform application initialization.
	if (!initInstance(nCmdShow))
	{
		throw EXCEPTION(L"Failed to initialize application instance");
	}

	// Set up OpenGL context for our window.
	if (!setupOpenGlContext())
	{
		assert(false); throw EXCEPTION_FMT(L"Failed to set up OpenGL context (version %d.%d)", m_openGlInfo.OpenGlMajor, m_openGlInfo.OpenGlMinor);
	}

	// Initial scale factor for the camera.
	const GLfloat CameraScaleFactor = 1.0f;

	GLfloat aspectRatio = m_wndInfo.m_clientWidth / (GLfloat)m_wndInfo.m_clientHeight;

	m_spCamera = std::make_unique<Camera>(aspectRatio, CameraScaleFactor, 
		m_openGlInfo.FieldOfView, m_openGlInfo.FrustumNear, m_openGlInfo.FrustumFar);
}

WindowBase::~WindowBase()
{
}

ATOM WindowBase::registerClass()
{
	WNDCLASSEX wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = windowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(m_wndInfo.m_iconId));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(m_wndInfo.m_menuId);
	wcex.lpszClassName = m_szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(m_wndInfo.m_iconSmallId));

	return RegisterClassEx(&wcex);
}

BOOL WindowBase::initInstance(int nCmdShow)
{
	const DWORD WINDOW_STYLE = WS_POPUPWINDOW | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

	RECT wndRect = { 0, 0, m_wndInfo.m_clientWidth, m_wndInfo.m_clientHeight };

	if (!AdjustWindowRect(&wndRect, WINDOW_STYLE, FALSE))
	{
		std::wcerr << L"AdjustWindowRect() failed: " << GetLastError() << '\n';
		assert(false); return FALSE;
	}

	m_hWnd = CreateWindow(m_szWindowClass, m_szTitle, WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT,
		wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, nullptr, nullptr, m_hInstance, this);
	if (!m_hWnd)
	{
		std::wcerr << L"CreateWindow() failed: " << GetLastError() << '\n';
		assert(false); return FALSE;
	}

	ShowWindow(m_hWnd, nCmdShow);

	if (!UpdateWindow(m_hWnd))
	{
		std::wcerr << L"UpdateWindow() failed\n";
		assert(false); return FALSE;
	}

	return TRUE;
}

bool WindowBase::setupOpenGlContext()
{
	if (m_openGlInfo.OpenGlMajor < 1)
	{
		std::wcerr << L"Invalid OpenGL major version: " << m_openGlInfo.OpenGlMajor << '\n';
		assert(false); return false;
	}

	HDC hDC = GetDC(m_hWnd);
	if (!hDC)
	{
		std::wcerr << L"Windows device context is NULL\n";
		assert(false); return false;
	}

	// Step 1. Set pixel format for the Windows DC.

	PIXELFORMATDESCRIPTOR pfd = {};

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags =
		PFD_SUPPORT_OPENGL |    // OpenGL window
		PFD_DRAW_TO_WINDOW |    // render to window
		PFD_DOUBLEBUFFER;       // support double-buffering
	pfd.iPixelType = PFD_TYPE_RGBA;       // color type: red, green, blue, and alpha
	pfd.cColorBits = 32;                  // preferred color depth (bits per pixel for each color buffer): 8, 16, 24, or 32
	pfd.cDepthBits = 24;                  // depth of the depth (z-axis) buffer
	//pfd.iLayerType = PFD_MAIN_PLANE;    // main layer. Ignored in OpenGL 3.0 and later

	// The OS/driver will try to find the matching pixel format.
	// If some value cannot be set, it will be replaced by the highest possible value (e.g. 24-bit colors instead of 32-bit).
	// Returns an integer ID of the pixel format.
	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (0 == pixelFormat)
	{
		std::wcerr << L"ChoosePixelFormat() failed: " << ::GetLastError() << '\n';
		assert(false); return false;
	}

	// Set the pixel format for the device context and the associated window.
	if (!SetPixelFormat(hDC, pixelFormat, &pfd))
	{
		std::wcerr << L"SetPixelFormat() failed: " << ::GetLastError() << '\n';
		assert(false); return false;
	}

	// Step 2. Create a temporary OpenGL rendering context to try to get the latest one - see below.
	HGLRC hRcTmp = wglCreateContext(hDC);
	if (!hRcTmp)
	{
		std::wcerr << L": wglCreateContext() failed: " << ::GetLastError() << '\n';
		assert(false); return false;
	}

	// Step 3. Make the temporary rendering context current for our thread.
	if (!wglMakeCurrent(hDC, hRcTmp))
	{
		std::wcerr << L": wglMakeCurrent() failed: " << ::GetLastError() << '\n';
		assert(false); return false;
	}

	// Step 4. Initialize GLEW (in particular, to be able to conveniently use the wglCreateContextAttribsARB extension.

	glewExperimental = true;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::wcerr << L"glewInit() failed: " << (char *)glewGetErrorString(err) << '\n';
		assert(false); return false;
	}
	std::wcout << L"GLEW library initialized, version " << (char *)glewGetString(GLEW_VERSION) << std::endl;

	// Step 5. Set up the modern OpenGL rendering context.

	// Set the OpenGL version required.
	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, m_openGlInfo.OpenGlMajor,
		WGL_CONTEXT_MINOR_VERSION_ARB, m_openGlInfo.OpenGlMinor,

#if _DEBUG
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
#endif

		0 };    // zero indicates the end of the array

	// If the pointer to this extension is NULL, the OpenGL version is not supported.
	if (!wglCreateContextAttribsARB)
	{
		std::wcerr << L": OpenGL version " << m_openGlInfo.OpenGlMajor << "." << m_openGlInfo.OpenGlMinor << " not supported\n";
		assert(false); return false;
	}

	// Create a modern OpenGL context.
	m_hRC = wglCreateContextAttribsARB(hDC, 0, attribs);
	if (!m_hRC)
	{
		std::wcerr << L"wglCreateContextAttribsARB() failed\n";
		assert(false); return false;
	}

	// On success, delete the temporary context.
	wglDeleteContext(hRcTmp);

	// Step 6. Make the final rendering context current for our thread.
	if (!wglMakeCurrent(hDC, m_hRC))
	{
		std::wcerr << L"wglMakeCurrent() failed (2): " << GetLastError() << '\n';
		assert(false); return false;
	}

	glDebugMessageCallback(openGlDebugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	std::wcout << L"OpenGL context (" << m_openGlInfo.OpenGlMajor << "." << m_openGlInfo.OpenGlMinor << ") set up" << std::endl;

	return true;
}

void APIENTRY WindowBase::openGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* param)
{
	UNREFERENCED_PARAMETER(length);
	UNREFERENCED_PARAMETER(param);

	std::wcout << "Debug message from the ";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		std::wcout << "OpenGL API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		std::wcout << "window system";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		std::wcout << "shader compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		std::wcout << "third party tools or libraries";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		std::wcout << "application (explicit)";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		std::wcout << "other source";
		break;
	default:    // unknown source?
		assert(false); break;
	}

	std::wcout << "\nMessage text: " << message;

	std::wcout << "\nType: ";
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		std::wcout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::wcout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::wcout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::wcout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::wcout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::wcout << "OTHER";
		break;
	}

	std::wcout << "\nID: " << id;

	std::wcout << "\nSeverity: ";
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_LOW:
		std::wcout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::wcout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		std::wcout << "HIGH";
		break;
	}
	std::wcout << std::endl;
}

int WindowBase::runMessageLoop()
{
	MSG msg = {};

	// Complete the window initialization.

	if (0 == m_wndInfo.m_clientHeight)    // prevent dividing by zero
	{
		m_wndInfo.m_clientHeight = 1;
	}

	if (!initialize())
	{
		std::wcerr << L"Failed to initialize derived window\n";
		return 1;
	}

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		render();
	}

	return (int)msg.wParam;
}

void WindowBase::resize(int clientWidth, int clientHeight)
{
	if (0 == clientHeight)    // prevent dividing by zero
	{
		clientHeight = 1;
	}

	// Resize the viewport.
	glViewport(0, 0, clientWidth, clientHeight);

	GLfloat aspectRatio = clientWidth / (GLfloat)clientHeight;

	// Calculate aspect ratio of the window.
	gluPerspective(m_openGlInfo.FieldOfView, aspectRatio, m_openGlInfo.FrustumNear, m_openGlInfo.FrustumFar);

	m_spCamera->resize(aspectRatio);
}

void WindowBase::translateCameraX(GLfloat diff)
{
	m_spCamera->translateX(diff);
}

void WindowBase::translateCameraY(GLfloat diff)
{
	m_spCamera->translateY(diff);
}

void WindowBase::translateCameraZ(GLfloat diff)
{
	m_spCamera->translateZ(diff);
}

void WindowBase::rotateCameraX(GLfloat angleDegrees)
{
	m_spCamera->rotateX(angleDegrees);
}

void WindowBase::rotateCameraY(GLfloat angleDegrees)
{
	m_spCamera->rotateY(angleDegrees);
}

void WindowBase::rotateCameraZ(GLfloat angleDegrees)
{
	m_spCamera->rotateZ(angleDegrees);
}

void WindowBase::rotateCameraXY(GLfloat xAngleDegrees, GLfloat yAngleDegrees)
{
	m_spCamera->rotateXY(xAngleDegrees, yAngleDegrees);
}

GLfloat WindowBase::getCameraScale() const
{
	return m_spCamera->getScale();
}

void WindowBase::scaleCamera(GLfloat amount)
{
	m_spCamera->scale(amount);
}

INT_PTR CALLBACK WindowBase::aboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

LRESULT CALLBACK WindowBase::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static WindowBase *pMainWnd = nullptr;

	switch (message)
	{
	case WM_CREATE:
		pMainWnd = (WindowBase *)((LPCREATESTRUCT)lParam)->lpCreateParams;
		assert(pMainWnd);
		break;
	case WM_SIZE:
		{
			WORD newClientWidth  = LOWORD(lParam);
			WORD newClientHeight = HIWORD(lParam);

			pMainWnd->resize(newClientWidth, newClientHeight);
		}
		break;
	case WM_KEYDOWN:
		pMainWnd->windowProcDerived(hWnd, message, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
