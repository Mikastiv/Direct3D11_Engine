#pragma once

#include "WinDefines.hpp"
#include "MikastivException.hpp"

#include <d3d11.h>

class Graphics
{
public:
    class Exception : public MikastivException
    {
    private:
        HRESULT hr{};

    public:
        Exception(int line, const char* file, HRESULT hr) noexcept;
        auto what() const noexcept -> const char* override;
        auto get_type() const noexcept -> const char* override;
        auto get_error_code() const noexcept -> HRESULT;
        auto get_error_description() const noexcept -> std::string;
    };

    class DeviceRemovedException : public Exception
    {
    public:
        DeviceRemovedException(int line, const char* file, HRESULT hr) noexcept;
        auto get_type() const noexcept -> const char* override;
    };

private:
    ID3D11Device* p_device = nullptr;
    ID3D11DeviceContext* p_context = nullptr;
    IDXGISwapChain* p_swap = nullptr;
    ID3D11RenderTargetView* p_target = nullptr;

public:
    explicit Graphics(HWND h_wnd);
    Graphics(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;
    ~Graphics();
    auto operator=(const Graphics&) -> Graphics& = delete;
    auto operator=(Graphics &&) -> Graphics& = delete;
    auto end_frame() -> void;
    auto clear_buffer(float red, float green, float blue) noexcept -> void;
};