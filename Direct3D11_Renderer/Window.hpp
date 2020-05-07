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
        static auto GetName() noexcept -> const wchar_t*;
        static auto GetInstance() noexcept -> HINSTANCE;

    public:
        WindowClass(const WindowClass&) = delete;
        WindowClass(WindowClass&&) = delete;
        auto operator=(const WindowClass&) -> WindowClass& = delete;
        auto operator=(WindowClass &&) -> WindowClass& = delete;

    private:
        WindowClass() noexcept;
        ~WindowClass();

    private:
        static WindowClass wndClass;
        static constexpr const wchar_t* WndClassName = L"Direct3D11 Engine Window";

    private:
        HINSTANCE hInstance{};
    };

public:
    class Exception : public MikastivException
    {
    public:
        static auto translate_error_code(HRESULT hr) noexcept -> std::string;

    public:
        Exception(int line, const char* file, HRESULT hr) noexcept;
        auto what() const noexcept -> const char* override;
        auto GetType() const noexcept -> const char* override;
        auto GetErrorCode() const noexcept -> HRESULT;
        auto GetErrorDescription() const noexcept -> std::string;

    private:
        HRESULT hr{};
    };

    //class NoGfxException : public MikastivException
    //{
    //public:
    //    NoGfxException(int line, const char* file) noexcept;
    //    auto GetType() const noexcept -> const char* override;
    //};

public:
    Keyboard kbd{};
    Mouse mouse{};
    std::unique_ptr<Graphics> pGfx{};

public:
    static auto ProcessMessages() noexcept -> std::optional<int>;

public:
    Window(int width, int height, const wchar_t* name);
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    ~Window();
    auto operator=(const Window&) -> Window& = delete;
    auto operator=(Window &&) -> Window& = delete;
    auto SetTitle(const std::wstring& title) -> void;
    auto GetHWND() const -> HWND;

private:
    static auto CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> LRESULT;
    static auto CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> LRESULT;

private:
    auto HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> LRESULT;

private:
    int width = 0;
    int height = 0;
    HWND hWnd{};
};
