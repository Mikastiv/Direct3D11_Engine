#include "Graphics.hpp"
#include "Helpers.hpp"

#include <sstream>

using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")

#define GFX_EXCEPT_NOINFO(hr) Graphics::Exception(__LINE__, __FILE__, hr)

#define GFX_THROW_NOINFO(hrcall)                                                                                       \
    if (FAILED(hr = (hrcall)))                                                                                         \
    throw Graphics::Exception(__LINE__, __FILE__, hr)

#ifdef _DEBUG
#define GFX_EXCEPT(hr) Graphics::Exception(__LINE__, __FILE__, hr, info_manager.get_messages())
#define GFX_THROW_INFO(hrcall)                                                                                         \
    info_manager.set();                                                                                                \
    if (FAILED(hr = (hrcall)))                                                                                         \
    throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr)                                                                                  \
    Graphics::DeviceRemovedException(__LINE__, __FILE__, hr, info_manager.get_messages())
#else
#define GFX_EXCEPT(hr) Graphics::Exception(__LINE__, __FILE__, hr)
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)
#endif

Graphics::Exception::Exception(int line, const char* file, HRESULT hr, std::vector<std::string> info_msgs) noexcept
    : MikastivException(line, file)
    , hr(hr)
{
    for (const auto& m : info_msgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}

auto Graphics::Exception::what() const noexcept -> const char*
{
    std::ostringstream oss;
    oss << get_type() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << get_error_code() << std::dec << " ("
        << (unsigned long)get_error_code() << ")\n"
        << "[Description] " << get_error_description() << '\n';
    if (!info.empty())
    {
        oss << "\n[Error Info]\n" << get_error_info() << "\n\n";
    }
    oss << get_origin_string();

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

auto Graphics::Exception::get_error_info() const noexcept -> std::string
{
    return info;
}

Graphics::DeviceRemovedException::DeviceRemovedException(int line,
                                                         const char* file,
                                                         HRESULT hr,
                                                         std::vector<std::string> info_msgs) noexcept
    : Exception(line, file, hr, info_msgs)
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
    sd.BufferCount = 2;
    sd.OutputWindow = h_wnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = 0;

    UINT swap_create_flags = 0u;
#ifdef _DEBUG
    swap_create_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr{};
    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(nullptr,
                                                 D3D_DRIVER_TYPE_HARDWARE,
                                                 nullptr,
                                                 swap_create_flags,
                                                 nullptr,
                                                 0,
                                                 D3D11_SDK_VERSION,
                                                 &sd,
                                                 &p_swap,
                                                 &p_device,
                                                 nullptr,
                                                 &p_context));

    ComPtr<ID3D11Resource> p_back_buffer{};
    GFX_THROW_INFO(p_swap->GetBuffer(0u, __uuidof(ID3D11Resource), &p_back_buffer));
    GFX_THROW_INFO(p_device->CreateRenderTargetView(p_back_buffer.Get(), nullptr, &p_target));
}

auto Graphics::end_frame() -> void
{
    HRESULT hr{};
#ifdef _DEBUG
    info_manager.set();
#endif

    if (FAILED(hr = p_swap->Present(1u, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(p_device->GetDeviceRemovedReason());
        }
        else
        {
            throw GFX_EXCEPT(hr);
        }
    }
}

auto Graphics::clear_buffer(float red, float green, float blue) noexcept -> void
{
    const float color[] = { red, green, blue, 1.0f };
    p_context->ClearRenderTargetView(p_target.Get(), color);
}
