#pragma once

#include "WinDefines.hpp"

class Window
{
private:
    class WindowClass
    {
    private:
        static WindowClass wnd_class;
        static constexpr const wchar_t* wnd_class_name = L"Direct3D11 Engine Window";

    private:
        HINSTANCE h_instance;

    private:
        WindowClass() noexcept;
        ~WindowClass();

    public:
        WindowClass(const WindowClass&) = delete;
        WindowClass(WindowClass&&) = delete;
        auto operator=(const WindowClass&) -> WindowClass& = delete;
        auto operator=(WindowClass &&) -> WindowClass& = delete;

    public:
        static auto get_name() noexcept -> const wchar_t*;
        static auto get_instance() noexcept -> HINSTANCE;
    };

private:
    int width;
    int height;
    HWND h_win;

private:
    static auto CALLBACK handle_msg_setup(HWND h_win, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT;
    static auto CALLBACK handle_msg_thunk(HWND h_win, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT;
    auto handle_msg(HWND h_win, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT;

public:
    Window(int width, int height, const wchar_t* name) noexcept;
    ~Window();
    Window(const Window&) = delete;
    Window(Window&&) = default;
    auto operator=(const Window&) -> Window& = delete;
    auto operator=(Window &&) -> Window& = default;
};
