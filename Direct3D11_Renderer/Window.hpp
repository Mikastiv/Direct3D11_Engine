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
    public:
        static auto get_name() noexcept -> const wchar_t*;
        static auto get_instance() noexcept -> HINSTANCE;

    public:
        WindowClass(const WindowClass&) = delete;
        WindowClass(WindowClass&&) = delete;
        auto operator=(const WindowClass&) -> WindowClass& = delete;
        auto operator=(WindowClass &&) -> WindowClass& = delete;

    private:
        WindowClass() noexcept;
        ~WindowClass();

    private:
        static WindowClass wnd_class;
        static constexpr const wchar_t* wnd_class_name = L"Direct3D11 Engine Window";

    private:
        HINSTANCE h_instance{};
    };

public:
    class Exception : public MikastivException
    {
    public:
        static auto translate_error_code(HRESULT hr) noexcept -> std::string;

    public:
        Exception(int line, const char* file, HRESULT hr) noexcept;
        auto what() const noexcept -> const char* override;
        auto get_type() const noexcept -> const char* override;
        auto get_error_code() const noexcept -> HRESULT;
        auto get_error_description() const noexcept -> std::string;

    private:
        HRESULT hr{};
    };

    class NoGfxException : public MikastivException
    {
    public:
        NoGfxException(int line, const char* file) noexcept;
        auto get_type() const noexcept -> const char* override;
    };

public:
    Keyboard kbd{};
    Mouse mouse{};
    std::unique_ptr<Graphics> p_gfx{};

public:
    static auto process_messages() noexcept -> std::optional<int>;

public:
    Window(int width, int height, const wchar_t* name);
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    ~Window();
    auto operator=(const Window&) -> Window& = delete;
    auto operator=(Window &&) -> Window& = delete;
    auto set_title(const std::wstring& title) -> void;
    auto create_gfx() -> void;
    auto has_gfx() const -> bool;
    auto gfx() -> Graphics&;

private:
    static auto CALLBACK handle_msg_setup(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT;
    static auto CALLBACK handle_msg_thunk(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT;

private:
    auto handle_msg(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param) noexcept -> LRESULT;

private:
    int width = 0;
    int height = 0;
    HWND h_wnd{};
};
