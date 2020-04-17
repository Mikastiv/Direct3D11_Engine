#include "Window.hpp"
#include "Helpers.hpp"
#include "resource.h"

#include <sstream>

Window::WindowClass Window::WindowClass::wnd_class;

Window::WindowClass::WindowClass() noexcept
    : h_instance(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = handle_msg_setup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = get_instance();
    wc.hIcon = static_cast<HICON>(LoadImage(get_instance(), MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, 0));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = get_name();
    wc.hIconSm = static_cast<HICON>(LoadImage(get_instance(), MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, 0));
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wnd_class_name, get_instance());
}

auto Window::WindowClass::get_name() noexcept -> const wchar_t*
{
    return wnd_class_name;
}

auto Window::WindowClass::get_instance() noexcept -> HINSTANCE
{
    return wnd_class.h_instance;
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

    h_wnd = CreateWindow(WindowClass::get_name(),
                         name,
                         WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         wr.right - wr.left,
                         wr.bottom - wr.top,
                         nullptr,
                         nullptr,
                         WindowClass::get_instance(),
                         this);

    ShowWindow(h_wnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
    DestroyWindow(h_wnd);
}

auto Window::set_title(const std::wstring& title) -> void
{
    if (SetWindowText(h_wnd, title.c_str()) == 0)
    {
        throw MKWND_LAST_EXCEPT();
    }
}

auto Window::create_gfx() -> void
{
    p_gfx = std::make_unique<Graphics>(h_wnd);
}

auto Window::has_gfx() const -> bool
{
    return !!p_gfx;
}

auto Window::gfx() -> Graphics&
{
    if (!p_gfx)
    {
        throw MKWND_NOGFX_EXCEPT();
    }
    return *p_gfx;
}

auto WINAPI Window::handle_msg_setup(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT
{
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(l_param);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window class
        SetWindowLongPtr(h_wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(h_wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handle_msg_thunk));
        // forward message to window class handler
        return pWnd->handle_msg(h_wnd, msg, w_param, l_param);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(h_wnd, msg, w_param, l_param);
}

auto WINAPI Window::handle_msg_thunk(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT
{
    // retrieve ptr to window instance
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(h_wnd, GWLP_USERDATA));
    // forward message to window instance handler

    return pWnd->handle_msg(h_wnd, msg, w_param, l_param);
}

auto Window::process_messages() noexcept -> std::optional<int>
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

auto Window::handle_msg(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT
{
    switch (msg)
    {
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_KILLFOCUS:
    {
        kbd.clear_keystates();
        break;
    }
        // -------------------------------------------------------
        //                  Keyboard messages
        // -------------------------------------------------------
    case WM_KEYDOWN:
        [[fallthrough]];
    case WM_SYSKEYDOWN:
    {
        // Autorepeat is bit 30 flag in l_param
        if (!(l_param & (0b1 << 30)) || kbd.autorepeat_is_enabled())
        {
            kbd.on_key_press(static_cast<unsigned char>(w_param));
        }
        break;
    }
    case WM_KEYUP:
        [[fallthrough]];
    case WM_SYSKEYUP:
    {
        kbd.on_key_release(static_cast<unsigned char>(w_param));
        break;
    }
    case WM_CHAR:
    {
        kbd.on_char(static_cast<unsigned char>(w_param));
        break;
    }
        // -------------------------------------------------------

        // -------------------------------------------------------
        //                    Mouse messages
        // -------------------------------------------------------
    case WM_MOUSEMOVE:
    {
        const POINTS pt = MAKEPOINTS(l_param);
        // in client region -> log move, and log enter + capture mouse (if not previously in window)
        if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
        {
            mouse.on_mouse_move(pt.x, pt.y);
            if (!mouse.is_in_window())
            {
                SetCapture(h_wnd);
                mouse.on_mouse_enter();
            }
        }
        // not in client -> log move / maintain capture if button down
        else
        {
            if (w_param & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
            {
                mouse.on_mouse_move(pt.x, pt.y);
            }
            // button up -> release capture / log event for leaving
            else
            {
                ReleaseCapture();
                mouse.on_mouse_leave();
            }
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(l_param);
        mouse.on_left_pressed(pt.x, pt.y);
        SetForegroundWindow(h_wnd);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(l_param);
        mouse.on_right_pressed(pt.x, pt.y);
        break;
    }
    case WM_MBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(l_param);
        mouse.on_middle_pressed(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(l_param);
        mouse.on_left_released(pt.x, pt.y);
        // release mouse if outside of window
        if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
        {
            ReleaseCapture();
            mouse.on_mouse_leave();
        }
        break;
    }
    case WM_RBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(l_param);
        mouse.on_right_released(pt.x, pt.y);
        if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
        {
            ReleaseCapture();
            mouse.on_mouse_leave();
        }
        break;
    }
    case WM_MBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(l_param);
        mouse.on_middle_released(pt.x, pt.y);
        if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
        {
            ReleaseCapture();
            mouse.on_mouse_leave();
        }
        break;
    }
    case WM_MOUSEWHEEL:
    {
        const POINTS pt = MAKEPOINTS(l_param);
        const int delta = GET_WHEEL_DELTA_WPARAM(w_param);
        mouse.on_wheel_delta(pt.x, pt.y, delta);
        break;
    }
        // -------------------------------------------------------
    }

    return DefWindowProc(h_wnd, msg, w_param, l_param);
}

auto Window::Exception::translate_error_code(HRESULT hr) noexcept -> std::string
{
    wchar_t* p_msg_buf = nullptr;
    const DWORD msg_len =
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      nullptr,
                      hr,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      reinterpret_cast<LPWSTR>(&p_msg_buf),
                      0,
                      nullptr);

    if (msg_len == 0)
    {
        return "Unidentified error code";
    }

    std::string error_string = wchar_to_str(p_msg_buf);

    // free windows buffer
    LocalFree(p_msg_buf);

    return error_string;
}

Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
    : MikastivException(line, file)
    , hr(hr)
{
}

const char* Window::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << get_type() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << get_error_code() << std::dec << " ("
        << (unsigned long)get_error_code() << ")\n"
        << "[Description] " << get_error_description() << '\n'
        << get_origin_string();
    what_buffer = oss.str();
    return what_buffer.c_str();
}

auto Window::Exception::get_type() const noexcept -> const char*
{
    return "Mikastiv Window Exception";
}

auto Window::Exception::get_error_code() const noexcept -> HRESULT
{
    return hr;
}

auto Window::Exception::get_error_description() const noexcept -> std::string
{
    return translate_error_code(hr);
}

Window::NoGfxException::NoGfxException(int line, const char* file) noexcept
    : MikastivException(line, file)
{
}

auto Window::NoGfxException::get_type() const noexcept -> const char*
{
    return "Chili Window Exception [No Graphics]";
}
