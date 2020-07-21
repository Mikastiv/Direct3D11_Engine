#include "Window.hpp"
#include "Helpers.hpp"
#include "WindowMacros.hpp"
#include "resource.h"
#include "imgui/imgui_impl_win32.h"

#include <sstream>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
    : hInstance(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, 0));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, 0));
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(WndClassName, GetInstance());
}

auto Window::WindowClass::GetName() noexcept -> const wchar_t*
{
    return WndClassName;
}

auto Window::WindowClass::GetInstance() noexcept -> HINSTANCE
{
    return wndClass.hInstance;
}

// Window Stuff
Window::Window(int width, int height, const wchar_t* name)
    : width(width)
    , height(height)
{
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false) == 0)
    {
        throw MKWND_LAST_EXCEPT();
    }

    hWnd = CreateWindow(
        WindowClass::GetName(),
        name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        nullptr,
        nullptr,
        WindowClass::GetInstance(),
        this);

    ShowWindow(hWnd, SW_SHOWDEFAULT);

    ImGui_ImplWin32_Init(hWnd);
}

Window::~Window()
{
    ImGui_ImplWin32_Shutdown();
    DestroyWindow(hWnd);
}

auto Window::SetTitle(const std::wstring& title) -> void
{
    if (SetWindowText(hWnd, title.c_str()) == 0)
    {
        throw MKWND_LAST_EXCEPT();
    }
}

auto Window::GetHWND() const -> HWND
{
    return hWnd;
}

auto WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> LRESULT
{
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window class
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // forward message to window class handler
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

auto WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> LRESULT
{
    // retrieve ptr to window instance
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // forward message to window instance handler

    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

auto Window::ProcessMessages() noexcept -> std::optional<int>
{
    MSG msg{};

    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return (int)msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

auto Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> LRESULT
{
    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }

    const auto& imguiIO = ImGui::GetIO();

    switch (msg)
    {
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_KILLFOCUS:
    {
        kbd.ClearKeystates();
        break;
    }
        // -------------------------------------------------------
        //                  Keyboard messages
        // -------------------------------------------------------
    case WM_KEYDOWN:
        [[fallthrough]];
    case WM_SYSKEYDOWN:
    {
        if (imguiIO.WantCaptureKeyboard)
        {
            break;
        }
        // Autorepeat is bit 30 flag in lParam
        if (!(lParam & (0b1 << 30)) || kbd.AutorepeatIsEnabled())
        {
            kbd.OnKeyPress(static_cast<unsigned char>(wParam));
        }
        break;
    }
    case WM_KEYUP:
        [[fallthrough]];
    case WM_SYSKEYUP:
    {
        if (imguiIO.WantCaptureKeyboard)
        {
            break;
        }
        kbd.OnKeyRelease(static_cast<unsigned char>(wParam));
        break;
    }
    case WM_CHAR:
    {
        if (imguiIO.WantCaptureKeyboard)
        {
            break;
        }
        kbd.OnChar(static_cast<unsigned char>(wParam));
        break;
    }
        // -------------------------------------------------------

        // -------------------------------------------------------
        //                    Mouse messages
        // -------------------------------------------------------
    case WM_MOUSEMOVE:
    {
        if (imguiIO.WantCaptureMouse)
        {
            break;
        }

        const POINTS pt = MAKEPOINTS(lParam);
        // in client region -> log move, and log enter + capture mouse (if not previously in window)
        if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
        {
            mouse.OnMouseMove(pt.x, pt.y);
            if (!mouse.IsInWindow())
            {
                SetCapture(hWnd);
                mouse.OnMouseEnter();
            }
        }
        // not in client -> log move / maintain capture if button down
        else
        {
            if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
            {
                mouse.OnMouseMove(pt.x, pt.y);
            }
            // button up -> release capture / log event for leaving
            else
            {
                ReleaseCapture();
                mouse.OnMouseLeave();
            }
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
        SetForegroundWindow(hWnd);
        if (imguiIO.WantCaptureMouse)
        {
            break;
        }
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftPressed(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        if (imguiIO.WantCaptureMouse)
        {
            break;
        }
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightPressed(pt.x, pt.y);
        break;
    }
    case WM_MBUTTONDOWN:
    {
        if (imguiIO.WantCaptureMouse)
        {
            break;
        }
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnMiddlePressed(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONUP:
    {
        if (imguiIO.WantCaptureMouse)
        {
            break;
        }
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftReleased(pt.x, pt.y);
        // release mouse if outside of window
        if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
        {
            ReleaseCapture();
            mouse.OnMouseLeave();
        }
        break;
    }
    case WM_RBUTTONUP:
    {
        if (imguiIO.WantCaptureMouse)
        {
            break;
        }
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightReleased(pt.x, pt.y);
        if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
        {
            ReleaseCapture();
            mouse.OnMouseLeave();
        }
        break;
    }
    case WM_MBUTTONUP:
    {
        if (imguiIO.WantCaptureMouse)
        {
            break;
        }
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnMiddleReleased(pt.x, pt.y);
        if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
        {
            ReleaseCapture();
            mouse.OnMouseLeave();
        }
        break;
    }
    case WM_MOUSEWHEEL:
    {
        if (imguiIO.WantCaptureMouse)
        {
            break;
        }
        const POINTS pt = MAKEPOINTS(lParam);
        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        mouse.OnWheelDelta(pt.x, pt.y, delta);
        break;
    }
        // -------------------------------------------------------
    default:
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

auto Window::Exception::translate_error_code(HRESULT hr) noexcept -> std::string
{
    wchar_t* pMsgBuf = nullptr;
    const DWORD msgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPWSTR>(&pMsgBuf),
        0,
        nullptr);

    if (msgLen == 0)
    {
        return "Unidentified error code";
    }

    std::string error_string = WCharToString(pMsgBuf);

    // free windows buffer
    LocalFree(pMsgBuf);

    return error_string;
}

Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
    : MikastivException(line, file)
    , hr(hr)
{
}

auto Window::Exception::what() const noexcept -> const char*
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode() << std::dec << " ("
        << (unsigned long)GetErrorCode() << ")\n"
        << "[Description] " << GetErrorDescription() << '\n'
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

auto Window::Exception::GetType() const noexcept -> const char*
{
    return "Mikastiv Window Exception";
}

auto Window::Exception::GetErrorCode() const noexcept -> HRESULT
{
    return hr;
}

auto Window::Exception::GetErrorDescription() const noexcept -> std::string
{
    return translate_error_code(hr);
}

// Window::NoGfxException::NoGfxException(int line, const char* file) noexcept
//    : MikastivException(line, file)
//{
//}
//
// auto Window::NoGfxException::GetType() const noexcept -> const char*
//{
//    return "Chili Window Exception [No Graphics]";
//}
