#include "Graphics.hpp"

#pragma comment(lib, "d3d11.lib")

Graphics::Graphics(HWND h_wnd)
{
    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = h_wnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    D3D11CreateDeviceAndSwapChain(nullptr,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  nullptr,
                                  0,
                                  nullptr,
                                  0,
                                  D3D11_SDK_VERSION,
                                  &sd,
                                  &p_swap,
                                  &p_device,
                                  nullptr,
                                  &p_context);

    ID3D11Resource* p_back_buffer = nullptr;
    p_swap->GetBuffer(0u, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&p_back_buffer));
    p_device->CreateRenderTargetView(p_back_buffer, nullptr, &p_target);
    p_back_buffer->Release();
}

Graphics::~Graphics()
{
    if (p_target != nullptr)
    {
        p_target->Release();
    }
    if (p_context != nullptr)
    {
        p_context->Release();
    }
    if (p_swap != nullptr)
    {
        p_swap->Release();
    }
    if (p_device != nullptr)
    {
        p_device->Release();
    }
}

auto Graphics::end_frame() -> void
{
    p_swap->Present(1u, 0u);
}

auto Graphics::clear_buffer(float red, float green, float blue) noexcept -> void
{
    const float color[] = { red, green, blue, 1.0f };
    p_context->ClearRenderTargetView(p_target, color);
}
