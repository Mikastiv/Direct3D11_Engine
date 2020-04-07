#include "include/Window.hpp"

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
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = get_name();
    wc.hIconSm = nullptr;
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