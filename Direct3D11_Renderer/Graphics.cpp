#include "Graphics.hpp"
#include "Helpers.hpp"

#include <sstream>

#pragma comment(lib, "d3d11.lib")

#define GFX_THROW_FAILED(hrcall)                                                                                       \
    if (FAILED(hr = (hrcall)))                                                                                         \
    throw Graphics::Exception(__LINE__, __FILE__, hr)

#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)

Graphics::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
    : MikastivException(line, file)
    , hr(hr)
{
}

auto Graphics::Exception::what() const noexcept -> const char*
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

auto Graphics::Exception::get_type() const noexcept -> const char*
{
    return "Mikastiv Graphics Exception";
}

auto Graphics::Exception::get_error_code() const noexcept -> HRESULT
{
    return hr;
}

auto Graphics::Exception::get_error_description() const noexcept -> std::string
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

Graphics::DeviceRemovedException::DeviceRemovedException(int line, const char* file, HRESULT hr) noexcept
    : Exception(line, file, hr)
{
}

auto Graphics::DeviceRemovedException::get_type() const noexcept -> const char*
{
    return "Mikastiv Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

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

    HRESULT hr{};
    GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(nullptr,
                                                   D3D_DRIVER_TYPE_HARDWARE,
                                                   nullptr,
                                                   D3D11_CREATE_DEVICE_DEBUG,
                                                   nullptr,
                                                   0,
                                                   D3D11_SDK_VERSION,
                                                   &sd,
                                                   &p_swap,
                                                   &p_device,
                                                   nullptr,
                                                   &p_context));

    ID3D11Resource* p_back_buffer = nullptr;
    GFX_THROW_FAILED(p_swap->GetBuffer(0u, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&p_back_buffer)));
    GFX_THROW_FAILED(p_device->CreateRenderTargetView(p_back_buffer, nullptr, &p_target));
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
    HRESULT hr{};
    if (FAILED(hr = p_swap->Present(1u, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(p_device->GetDeviceRemovedReason());
        }
        else
        {
            GFX_THROW_FAILED(hr);
        }
    }
}

auto Graphics::clear_buffer(float red, float green, float blue) noexcept -> void
{
    const float color[] = { red, green, blue, 1.0f };
    p_context->ClearRenderTargetView(p_target, color);
}
