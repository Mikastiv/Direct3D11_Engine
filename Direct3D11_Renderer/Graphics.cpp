#include "Graphics.hpp"
#include "Helpers.hpp"

#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace WRL = Microsoft::WRL;
namespace DX = DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException(__LINE__, __FILE__, hr)

#define GFX_THROW_NOINFO(hrcall)                                                                                       \
    if (FAILED(hr = (hrcall)))                                                                                         \
    throw Graphics::HrException(__LINE__, __FILE__, hr)

#ifdef _DEBUG
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__, __FILE__, hr, info_manager.get_messages())
#define GFX_THROW_INFO(hrcall)                                                                                         \
    info_manager.set();                                                                                                \
    if (FAILED(hr = (hrcall)))                                                                                         \
    throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr)                                                                                  \
    Graphics::DeviceRemovedException(__LINE__, __FILE__, hr, info_manager.get_messages())
#define GFX_THROW_INFO_ONLY(fn_call)                                                                                   \
    info_manager.set();                                                                                                \
    fn_call;                                                                                                           \
    {                                                                                                                  \
        auto v = info_manager.get_messages();                                                                          \
        if (!v.empty())                                                                                                \
        {                                                                                                              \
            throw Graphics::InfoException(__LINE__, __FILE__, v);                                                      \
        }                                                                                                              \
    }
#else
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__, __FILE__, hr)
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)
#define GFX_THROW_INFO_ONLY(fn_call) fn_call
#endif

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> info_msgs) noexcept
    : Exception(line, file)
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

auto Graphics::HrException::what() const noexcept -> const char*
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

auto Graphics::HrException::get_type() const noexcept -> const char*
{
    return "Mikastiv Graphics Exception";
}

auto Graphics::HrException::get_error_code() const noexcept -> HRESULT
{
    return hr;
}

auto Graphics::HrException::get_error_description() const noexcept -> std::string
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

auto Graphics::HrException::get_error_info() const noexcept -> std::string
{
    return info;
}

Graphics::DeviceRemovedException::DeviceRemovedException(int line,
                                                         const char* file,
                                                         HRESULT hr,
                                                         std::vector<std::string> info_msgs) noexcept
    : HrException(line, file, hr, info_msgs)
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

    WRL::ComPtr<ID3D11Resource> p_back_buffer{};
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

auto Graphics::draw_test_triangle(float angle, int x, int y) -> void
{
    HRESULT hr{};

    struct Vertex
    {
        struct
        {
            float x;
            float y;
        } pos;

        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        } color;
    };

    const Vertex vertices[]{ { 0.0f, 0.5f, 255u, 255u, 0u },  { 0.5f, -0.5f, 255u, 145u, 0u },
                             { -0.5f, -0.5f, 0u, 0u, 255u },  { -0.3f, 0.25f, 255u, 255u, 0u },
                             { 0.3f, 0.25f, 255u, 145u, 0u }, { 0.0f, -0.75f, 0u, 0u, 255u } };

    const uint16_t indices[]{ 0u, 1u, 2u, 0u, 4u, 1u, 1u, 5u, 2u, 0u, 2u, 3u };

    WRL::ComPtr<ID3D11Buffer> p_vertex_buffer{};

    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = sizeof(vertices);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0u;
    desc.MiscFlags = 0u;
    desc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA data{};
    data.pSysMem = vertices;

    GFX_THROW_INFO(p_device->CreateBuffer(&desc, &data, &p_vertex_buffer));
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    p_context->IASetVertexBuffers(0u, 1u, p_vertex_buffer.GetAddressOf(), &stride, &offset);

    WRL::ComPtr<ID3D11Buffer> p_index_buffer{};

    D3D11_BUFFER_DESC desc1{};
    desc1.ByteWidth = sizeof(indices);
    desc1.Usage = D3D11_USAGE_DEFAULT;
    desc1.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc1.CPUAccessFlags = 0u;
    desc1.MiscFlags = 0u;
    desc1.StructureByteStride = sizeof(uint16_t);

    D3D11_SUBRESOURCE_DATA data1{};
    data1.pSysMem = indices;

    GFX_THROW_INFO(p_device->CreateBuffer(&desc1, &data1, &p_index_buffer));
    p_context->IASetIndexBuffer(p_index_buffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

    struct ConstBuffer
    {
        DX::XMMATRIX transform;
    };

    const float xcoord = (x / (Graphics::screen_width / 2.0f) - 1.0f);
    const float ycoord = -(y / (Graphics::screen_height / 2.0f) - 1.0f);

    const ConstBuffer cb{ DX::XMMatrixTranspose(DX::XMMatrixRotationZ(angle)
                                                * DX::XMMatrixScaling(9.0f / 16.0f, 1.0f, 1.0f)
                                                * DX::XMMatrixTranslation(xcoord, ycoord, 0.0f)) };

    WRL::ComPtr<ID3D11Buffer> p_const_buffer{};
    D3D11_BUFFER_DESC cbd{};
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0u;
    cbd.ByteWidth = sizeof(cb);
    cbd.StructureByteStride = 0u;

    D3D11_SUBRESOURCE_DATA data2{};
    data2.pSysMem = &cb;

    GFX_THROW_INFO(p_device->CreateBuffer(&cbd, &data2, &p_const_buffer));

    p_context->VSSetConstantBuffers(0u, 1u, p_const_buffer.GetAddressOf());

    p_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    WRL::ComPtr<ID3D11VertexShader> p_vertex_shader{};
    WRL::ComPtr<ID3DBlob> p_blob{};
    GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &p_blob));
    GFX_THROW_INFO(
        p_device->CreateVertexShader(p_blob->GetBufferPointer(), p_blob->GetBufferSize(), nullptr, &p_vertex_shader));
    p_context->VSSetShader(p_vertex_shader.Get(), nullptr, 0u);

    WRL::ComPtr<ID3D11InputLayout> p_input_layout{};
    const D3D11_INPUT_ELEMENT_DESC ied[]{
        { "POSITION", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
        { "COLOR", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u },
    };
    GFX_THROW_INFO(p_device->CreateInputLayout(
        ied, (UINT)std::size(ied), p_blob->GetBufferPointer(), p_blob->GetBufferSize(), &p_input_layout));
    p_context->IASetInputLayout(p_input_layout.Get());

    WRL::ComPtr<ID3D11PixelShader> p_pixel_shader{};
    GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &p_blob));
    GFX_THROW_INFO(
        p_device->CreatePixelShader(p_blob->GetBufferPointer(), p_blob->GetBufferSize(), nullptr, &p_pixel_shader));
    p_context->PSSetShader(p_pixel_shader.Get(), nullptr, 0u);

    p_context->OMSetRenderTargets(1u, p_target.GetAddressOf(), nullptr);

    D3D11_VIEWPORT vp[]{ { 0.0f, 0.0f, (FLOAT)screen_width, (FLOAT)screen_height, 0.0f, 1.0f } };

    p_context->RSSetViewports(1u, &vp[0]);
    GFX_THROW_INFO_ONLY(p_context->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> info_msgs) noexcept
    : Exception(line, file)
{
    for (const auto& m : info_msgs)
    {
        info += m;
        info.push_back('\n');
    }
    if (!info.empty())
    {
        info.pop_back();
    }
}

auto Graphics::InfoException::what() const noexcept -> const char*
{
    std::ostringstream oss;
    oss << get_type() << "\n\n[Error Info]\n" << get_error_info() << "\n\n";
    oss << get_origin_string();
    what_buffer = oss.str();
    return what_buffer.c_str();
}

auto Graphics::InfoException::get_type() const noexcept -> const char*
{
    return "Mikastiv Graphics Info Exception";
}

auto Graphics::InfoException::get_error_info() const noexcept -> std::string
{
    return info;
}
