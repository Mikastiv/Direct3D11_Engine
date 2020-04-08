#include "Window.hpp"
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
    wc.hIcon = static_cast<HICON>(LoadImage(get_instance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = get_name();
    wc.hIconSm = static_cast<HICON>(LoadImage(get_instance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
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
Window::Window(int width, int height, const wchar_t* name) noexcept
    : width(width)
    , height(height)
{
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false);

    h_win = CreateWindow(WindowClass::get_name(),
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

    ShowWindow(h_win, SW_SHOWDEFAULT);
}

Window::~Window()
{
    DestroyWindow(h_win);
}

auto WINAPI Window::handle_msg_setup(HWND h_win, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT
{
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(l_param);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window class
        SetWindowLongPtr(h_win, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(h_win, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handle_msg_thunk));
        // forward message to window class handler
        return pWnd->handle_msg(h_win, msg, w_param, l_param);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(h_win, msg, w_param, l_param);
}

auto WINAPI Window::handle_msg_thunk(HWND h_win, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT
{
    // retrieve ptr to window class
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(h_win, GWLP_USERDATA));
    // forward message to window class handler
    return pWnd->handle_msg(h_win, msg, w_param, l_param);
}

auto Window::handle_msg(HWND h_win, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(h_win, msg, w_param, l_param);
}

auto Window::Exception::translate_error_code(HRESULT hr) noexcept -> std::string
{
    wchar_t* p_msg_buf = nullptr;
    DWORD msg_len =
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

    std::wstring tmp = p_msg_buf;
    std::string error_string{};
    for (const auto& c : tmp)
    {
        error_string += (char)c;
    }

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
    oss << get_type() << '\n'
        << "[Error Code] " << get_error_code() << '\n'
        << "[Description] " << get_error_string() << '\n'
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

auto Window::Exception::get_error_string() const noexcept -> std::string
{
    return translate_error_code(hr);
}
