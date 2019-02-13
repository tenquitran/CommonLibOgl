#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;

//////////////////////////////////////////////////////////////////////////


WindowMain::WindowMain(HINSTANCE hInstance, const WindowInfo& wndInfo, const OpenGLInfo& openGLInfo, std::weak_ptr<OglScene>& scene)
    : m_hInstance(hInstance), m_wndInfo(wndInfo), m_openGlInfo(openGLInfo), m_spScene(scene)
{
    if (!m_hInstance)
    {
        ATLASSERT(FALSE); throw EXCEPTION(L"Application instance is NULL");
    }
    else if (   m_wndInfo.m_clientWidth  < 1
             || m_wndInfo.m_clientHeight < 1)
    {
        ATLASSERT(FALSE); throw EXCEPTION_FMT(L"Invalid window size: (%d, %d)", m_wndInfo.m_clientWidth, m_wndInfo.m_clientHeight);
    }
    else if (!m_spScene)
    {
        ATLASSERT(FALSE); throw EXCEPTION(L"OpenGL scene is NULL");
    }

    // Initialize global strings.
    LoadString(hInstance, m_wndInfo.m_titleId, m_szTitle, MAX_LOADSTRING);
    LoadString(hInstance, m_wndInfo.m_wndClassId, m_szWindowClass, MAX_LOADSTRING);

    if (!registerClass())
    {
        ATLASSERT(FALSE); throw EXCEPTION(L"Failed to register windows class");
    }
}

WindowMain::~WindowMain()
{
    if (m_hWndTemporary)
    {
        DestroyWindow(m_hWndTemporary);
    }
}

GLuint WindowMain::getGlslProgramId() const
{
    if (!m_spScene)
    {
        ATLASSERT(FALSE); return 0;
    }

    return m_spScene->getGlslProgramId();
}

ATOM WindowMain::registerClass()
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

    if (!m_wndInfo.m_menuId)
    {
        wcex.lpszMenuName = nullptr;
    }
    else
    {
        wcex.lpszMenuName = MAKEINTRESOURCE(m_wndInfo.m_menuId);
    }

    wcex.lpszClassName = m_szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(m_wndInfo.m_iconSmallId));

    return RegisterClassEx(&wcex);
}

BOOL WindowMain::initInstance(int nCmdShow)
{
    const DWORD WindowStyle = WS_POPUPWINDOW | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

    RECT wndRect = { 0, 0, m_wndInfo.m_clientWidth, m_wndInfo.m_clientHeight };

    if (!AdjustWindowRect(&wndRect, WindowStyle, FALSE))
    {
        std::wcerr << L"AdjustWindowRect() failed: " << GetLastError() << '\n';
        ATLASSERT(FALSE); return FALSE;
    }

    m_hWnd = CreateWindow(m_szWindowClass, m_szTitle, WindowStyle, CW_USEDEFAULT, CW_USEDEFAULT,
        wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, nullptr, nullptr, m_hInstance, this);
    if (!m_hWnd)
    {
        std::wcerr << L"CreateWindow() failed: " << GetLastError() << '\n';
        ATLASSERT(FALSE); return FALSE;
    }

    ShowWindow(m_hWnd, nCmdShow);

    if (!UpdateWindow(m_hWnd))
    {
        std::wcerr << L"UpdateWindow() failed\n";
        ATLASSERT(FALSE); return FALSE;
    }

    return TRUE;
}

bool WindowMain::setupOpenGlContext()
{
    if (m_openGlInfo.OpenGlMajor < 1)
    {
        std::wcerr << L"Invalid OpenGL major version: " << m_openGlInfo.OpenGlMajor << '\n';
        ATLASSERT(FALSE); return false;
    }

    // Create temporary window and get its device context.

    const DWORD WindowStyle = WS_POPUPWINDOW | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

    m_hWndTemporary = CreateWindow(m_szWindowClass, L"Title", WindowStyle, 0, 0, 1, 1, nullptr, nullptr, m_hInstance, this);

    HDC tempDC = GetDC(m_hWndTemporary);

    // Step 1. Set pixel format for the window's DC.

    PIXELFORMATDESCRIPTOR tempPfd = {};

    tempPfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    tempPfd.nVersion = 1;
    tempPfd.dwFlags =
        PFD_SUPPORT_OPENGL |    // OpenGL window
        PFD_DRAW_TO_WINDOW |    // render to window
        PFD_DOUBLEBUFFER;       // support double-buffering
    tempPfd.iPixelType = PFD_TYPE_RGBA;       // color type: red, green, blue, and alpha
    tempPfd.cColorBits = 32;                  // preferred color depth (bits per pixel for each color buffer): 8, 16, 24, or 32
    tempPfd.cDepthBits = 24;                  // depth of the depth (z-axis) buffer
    tempPfd.cAlphaBits = 8;
    //pfd.iLayerType = PFD_MAIN_PLANE;    // main layer. Ignored in OpenGL 3.0 and later

    // The OS/driver will try to find the matching pixel format.
    // If some value cannot be set, it will be replaced by the highest possible value (e.g. 24-bit colors instead of 32-bit).
    // Returns an integer ID of the pixel format.
    int pixelFormat = ChoosePixelFormat(tempDC, &tempPfd);
    if (0 == pixelFormat)
    {
        std::wcerr << L"ChoosePixelFormat() failed: " << ::GetLastError() << '\n';
        ATLASSERT(FALSE); return false;
    }

    // Set the pixel format for the device context and the associated window.
    if (!SetPixelFormat(tempDC, pixelFormat, &tempPfd))
    {
        std::wcerr << L"SetPixelFormat() failed: " << ::GetLastError() << '\n';
        ATLASSERT(FALSE); return false;
    }

    // Step 2. Create a temporary OpenGL rendering context to try to get the latest one - see below.
    HGLRC hTempRC = wglCreateContext(tempDC);
    if (!hTempRC)
    {
        std::wcerr << L": wglCreateContext() failed: " << ::GetLastError() << '\n';
        ATLASSERT(FALSE); return false;
    }

    // Step 3. Make the temporary rendering context current for our thread.
    if (!wglMakeCurrent(tempDC, hTempRC))
    {
        std::wcerr << L": wglMakeCurrent() failed: " << ::GetLastError() << '\n';
        ATLASSERT(FALSE); return false;
    }

    // Step 4. Initialize GLEW (in particular, to be able to conveniently use the wglCreateContextAttribsARB extension.

    glewExperimental = true;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::wcerr << L"glewInit() failed: " << (char *)glewGetErrorString(err) << '\n';
        ATLASSERT(FALSE); return false;
    }
    std::wcout << L"GLEW library initialized, version " << (char *)glewGetString(GLEW_VERSION) << std::endl;

    // Step 5. Set up the modern OpenGL rendering context.

    // Use the maximum allowed number of samples for anti-aliasing.
    GLint maxSamples = {};
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);

    int attribs[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_SAMPLE_BUFFERS_ARB, 1,      // number of buffers for multisampling (must be 1)
        WGL_SAMPLES_ARB, maxSamples,    // number of samples
        0 };    // zero indicates the end of the array

    int pixelFormat2 = {};
    UINT numFormats = {};

    HDC hDC = GetDC(m_hWnd);
    if (!hDC)
    {
        std::wcerr << L"Windows device context is NULL\n";
        ATLASSERT(FALSE); return false;
    }

    if (!wglChoosePixelFormatARB(hDC, attribs, nullptr, 1, &pixelFormat2, &numFormats)
        || 0 == numFormats)
    {
        std::wcerr << L"wglChoosePixelFormatARB() failed\n";
        ATLASSERT(FALSE); return false;
    }

    PIXELFORMATDESCRIPTOR pfd = {};

    if (0 == DescribePixelFormat(hDC, pixelFormat2, sizeof(pfd), &pfd))
    {
        std::wcerr << L"DescribePixelFormat() failed: " << GetLastError() << '\n';
        ATLASSERT(FALSE); return false;
    }

    if (!SetPixelFormat(hDC, pixelFormat2, &pfd))
    {
        std::wcerr << L"SetPixelFormat() failed: " << GetLastError() << '\n';
        ATLASSERT(FALSE); return false;
    }

    // Create a modern OpenGL context.

    int contextAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, m_openGlInfo.OpenGlMajor,
        WGL_CONTEXT_MINOR_VERSION_ARB, m_openGlInfo.OpenGlMinor,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,

#if _DEBUG
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
        0
    };

    // If the pointer to this extension is NULL, the OpenGL version is not supported.
    if (!wglCreateContextAttribsARB)
    {
        std::wcerr << L": OpenGL version " << m_openGlInfo.OpenGlMajor << "." << m_openGlInfo.OpenGlMinor << " not supported\n";
        ATLASSERT(FALSE); return false;
    }

    m_hRC = wglCreateContextAttribsARB(hDC, 0, contextAttribs);
    if (!m_hRC)
    {
        std::wcerr << L"wglCreateContextAttribsARB() failed\n";
        ATLASSERT(FALSE); return false;
    }

    // On success, delete the temporary context.
    wglDeleteContext(hTempRC);

    // Step 6. Make the final rendering context current for our thread.
    if (!wglMakeCurrent(hDC, m_hRC))
    {
        std::wcerr << L"wglMakeCurrent() failed (2): " << GetLastError() << '\n';
        ATLASSERT(FALSE); return false;
    }

    ReleaseDC(m_hWndTemporary, tempDC);

    glDebugMessageCallback(openGlDebugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    std::wcout << L"OpenGL context (" << m_openGlInfo.OpenGlMajor << "." << m_openGlInfo.OpenGlMinor << ") set up" << std::endl;

    return true;
}

void APIENTRY WindowMain::openGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
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
        ATLASSERT(FALSE); break;
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

int WindowMain::runMessageLoop(int nCmdShow, const ShaderCollection& shaders, const glm::vec3& cameraOffset /*= {}*/)
{
    // Perform application initialization.
    if (!initInstance(nCmdShow))
    {
        std::wcerr << L"Failed to initialize application instance\n";
        ATLASSERT(FALSE); return 1;
    }

    // Set up OpenGL context for our window.
    if (!setupOpenGlContext())
    {
        std::wcerr << L"Failed to set up OpenGL context (version " << m_openGlInfo.OpenGlMajor << L"." << m_openGlInfo.OpenGlMinor << L")\n";
        ATLASSERT(FALSE); return 1;
    }

    MSG msg = {};

    // Complete the window initialization.

    if (0 == m_wndInfo.m_clientHeight)    // prevent dividing by zero
    {
        m_wndInfo.m_clientHeight = 1;
    }

    GLfloat aspectRatio = m_wndInfo.m_clientWidth / (GLfloat)m_wndInfo.m_clientHeight;

    if (!m_spScene->initialize(aspectRatio, m_openGlInfo, shaders, cameraOffset))
    {
        std::wcerr << L"Failed to initialize OpenGL scene\n";
        ATLASSERT(FALSE); return 1;
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

void WindowMain::resize(int clientWidth, int clientHeight)
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

    m_spScene->resize(aspectRatio);
}

INT_PTR CALLBACK WindowMain::aboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

LRESULT CALLBACK WindowMain::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static WindowMain *pMainWnd = nullptr;

    switch (message)
    {
    case WM_CREATE:
        // Our code creates first a real and then a fake window, so our main window pointer is initialized only once.
        if (!pMainWnd)
        {
            pMainWnd = (WindowMain *)((LPCREATESTRUCT)lParam)->lpCreateParams;
            ATLASSERT(pMainWnd);
        }
        break;
    case WM_SIZE:
        {
            WORD newClientWidth  = LOWORD(lParam);
            WORD newClientHeight = HIWORD(lParam);

            pMainWnd->resize(newClientWidth, newClientHeight);
        }
        break;
    case WM_KEYDOWN:
        // TODO: fix and uncomment
#if 0
        pMainWnd->windowProcDerived(hWnd, message, wParam, lParam);
#endif
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void WindowMain::render() const
{
    m_spScene->render();

    SwapBuffers(GetDC(m_hWnd));
}
