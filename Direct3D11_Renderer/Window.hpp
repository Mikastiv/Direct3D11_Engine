#pragma once

#include "WinDefines.hpp"
#include "MikastivException.hpp"
#include "Graphics.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"

#include <optional>
#include <memory>

class Window
{
private:
    class WindowClass
    {
    private:
        static WindowClass wnd_class;
        static constexpr const wchar_t* wnd_class_name = L"Direct3D11 Engine Window";

    private:
        HINSTANCE h_instance{};

    private:
        WindowClass() noexcept;
        ~WindowClass();

    public:
        static auto get_name() noexcept -> const wchar_t*;
        static auto get_instance() noexcept -> HINSTANCE;

    public:
        WindowClass(const WindowClass&) = delete;
        WindowClass(WindowClass&&) = delete;
        auto operator=(const WindowClass&) -> WindowClass& = delete;
        auto operator=(WindowClass &&) -> WindowClass& = delete;
    };

public:
    class Exception : public MikastivException
    {
    private:
        HRESULT hr{};

    public:
        static auto translate_error_code(HRESULT hr) noexcept -> std::string;

    public:
        Exception(int line, const char* file, HRESULT hr) noexcept;
        auto what() const noexcept -> const char* override;
        auto get_type() const noexcept -> const char* override;
        auto get_error_code() const noexcept -> HRESULT;
        auto get_error_string() const noexcept -> std::string;
    };

private:
    int width = 0;
    int height = 0;
    HWND h_wnd{};

public:
    Keyboard kbd{};
    Mouse mouse{};
    std::unique_ptr<Graphics> p_gfx{};

private:
    static auto CALLBACK handle_msg_setup(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT;
    static auto CALLBACK handle_msg_thunk(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT;

public:
    static auto process_messages() -> std::optional<int>;

private:
    auto handle_msg(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT;

public:
    Window(int width, int height, const wchar_t* name);
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    ~Window();
    auto operator=(const Window&) -> Window& = delete;
    auto operator=(Window &&) -> Window& = delete;
    auto set_title(const std::wstring& title) -> void;
    auto gfx() -> Graphics&;
};

#define MK_WND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define MK_WND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())