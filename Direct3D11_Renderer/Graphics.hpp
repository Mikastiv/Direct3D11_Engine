#pragma once

#include "WinDefines.hpp"

#include <d3d11.h>

class Graphics
{
private:
    ID3D11Device* p_device = nullptr;
    ID3D11DeviceContext* p_context = nullptr;
    IDXGISwapChain* p_swap = nullptr;
    ID3D11RenderTargetView* p_target = nullptr;

public:
    Graphics(HWND h_wnd);
    Graphics(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;
    ~Graphics();
    auto operator=(const Graphics&) -> Graphics& = delete;
    auto operator=(Graphics &&) -> Graphics& = delete;
    auto end_frame() -> void;
    auto clear_buffer(float red, float green, float blue) noexcept -> void;
};