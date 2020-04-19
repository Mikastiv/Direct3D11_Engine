#include "Graphics.hpp"
#include "Helpers.hpp"
#include "GraphicsMacros.hpp"

#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace WRL = Microsoft::WRL;
namespace DX = DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
    : Exception(line, file)
    , hr(hr)
{
    for (const auto& m : infoMsgs)
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
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode() << std::dec << " ("
        << (unsigned long)GetErrorCode() << ")\n"
        << "[Description] " << GetErrorDescription() << '\n';
    if (!info.empty())
    {
        oss << "\n[Error Info]\n" << GetErrorInfo() << "\n\n";
    }
    oss << GetOriginString();

    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

auto Graphics::HrException::GetType() const noexcept -> const char*
{
    return "Mikastiv Graphics Exception";
}

auto Graphics::HrException::GetErrorCode() const noexcept -> HRESULT
{
    return hr;
}

auto Graphics::HrException::GetErrorDescription() const noexcept -> std::string
{
    wchar_t* pMsgBuf = nullptr;
    const DWORD msgLen =
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      nullptr,
                      hr,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      reinterpret_cast<LPWSTR>(&pMsgBuf),
                      0,
                      nullptr);

    if (msgLen == 0)
    {
        return "Unidentified error code";
    }

    std::string errorString = WCharToString(pMsgBuf);

    // free windows buffer
    LocalFree(pMsgBuf);

    return errorString;
}

auto Graphics::HrException::GetErrorInfo() const noexcept -> std::string
{
    return info;
}

Graphics::DeviceRemovedException::DeviceRemovedException(int line,
                                                         const char* file,
                                                         HRESULT hr,
                                                         std::vector<std::string> infoMsgs) noexcept
    : HrException(line, file, hr, std::move(infoMsgs))
{
}

auto Graphics::DeviceRemovedException::GetType() const noexcept -> const char*
{
    return "Mikastiv Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics::Graphics(HWND hWnd)
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
    sd.OutputWindow = hWnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = 0;

    UINT swapCreateFlags = 0u;
#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr{};
    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(nullptr,
                                                 D3D_DRIVER_TYPE_HARDWARE,
                                                 nullptr,
                                                 swapCreateFlags,
                                                 nullptr,
                                                 0,
                                                 D3D11_SDK_VERSION,
                                                 &sd,
                                                 &pSwap,
                                                 &pDevice,
                                                 nullptr,
                                                 &pContext));

    WRL::ComPtr<ID3D11Resource> pBackBuffer{};
    GFX_THROW_INFO(pSwap->GetBuffer(0u, __uuidof(ID3D11Resource), &pBackBuffer));
    GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));

    D3D11_DEPTH_STENCIL_DESC dsDesc{};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    WRL::ComPtr<ID3D11DepthStencilState> pDSState{};
    GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

    pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

    WRL::ComPtr<ID3D11Texture2D> pDepthStencil{};
    D3D11_TEXTURE2D_DESC descDepth{};
    descDepth.Width = Graphics::ScreenWidth;
    descDepth.Height = Graphics::ScreenHeight;
    descDepth.MipLevels = 1u;
    descDepth.ArraySize = 1u;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1u;
    descDepth.SampleDesc.Quality = 0u;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    GFX_THROW_INFO(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSView{};
    descDSView.Format = DXGI_FORMAT_D32_FLOAT;
    descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSView.Texture2D.MipSlice = 0u;
    GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSView, &pDsv));
}

auto Graphics::EndFrame() -> void
{
    HRESULT hr{};
#ifndef NDEBUG
    infoManager.Set();
#endif

    if (FAILED(hr = pSwap->Present(1u, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        throw GFX_EXCEPT(hr);
    }
}

auto Graphics::ClearBuffer(float red, float green, float blue) noexcept -> void
{
    const float color[] = { red, green, blue, 1.0f };
    pContext->ClearRenderTargetView(pTarget.Get(), color);
    pContext->ClearDepthStencilView(pDsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

auto Graphics::DrawTestTriangle(float angle, int x, int y) -> void
{
    HRESULT hr{};

    struct Vertex
    {
        struct
        {
            float x;
            float y;
            float z;
        } pos;
    };

    const Vertex vertices[]{
        { -1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f }, { -1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, -1.0f },
        { -1.0f, -1.0f, 1.0f },  { 1.0f, -1.0f, 1.0f },  { -1.0f, 1.0f, 1.0f },  { 1.0f, 1.0f, 1.0f },
    };

    const uint16_t indices[]{ 0, 2, 1, 2, 3, 1, 1, 3, 5, 3, 7, 5, 2, 6, 3, 3, 6, 7,
                              4, 5, 7, 4, 7, 6, 0, 4, 2, 2, 4, 6, 0, 1, 4, 1, 5, 4 };

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

    GFX_THROW_INFO(pDevice->CreateBuffer(&desc, &data, &p_vertex_buffer));
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    pContext->IASetVertexBuffers(0u, 1u, p_vertex_buffer.GetAddressOf(), &stride, &offset);

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

    GFX_THROW_INFO(pDevice->CreateBuffer(&desc1, &data1, &p_index_buffer));
    pContext->IASetIndexBuffer(p_index_buffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

    struct ConstBuffer
    {
        DX::XMMATRIX transform;
    };

    const float xcoord = (x / (Graphics::ScreenWidth / 2.0f) - 1.0f);
    const float ycoord = -(y / (Graphics::ScreenHeight / 2.0f) - 1.0f);

    const auto ar = (float)Graphics::ScreenHeight / (float)Graphics::ScreenWidth;
    const ConstBuffer cb{ DX::XMMatrixTranspose(DX::XMMatrixRotationZ(angle) * DX::XMMatrixRotationX(angle)
                                                * DX::XMMatrixTranslation(xcoord, ycoord, 4.0f)
                                                * DX::XMMatrixPerspectiveLH(1.0f, ar, 0.5f, 10.0f)) };

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

    GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &data2, &p_const_buffer));

    pContext->VSSetConstantBuffers(0u, 1u, p_const_buffer.GetAddressOf());

    struct ConstBuffer2
    {
        struct
        {
            float r;
            float g;
            float b;
            float a;
        } face_colors[6];
    };
    const ConstBuffer2 cb2 = { {
        { 1.0f, 0.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 1.0f },
    } };

    WRL::ComPtr<ID3D11Buffer> p_const_buffer2{};
    D3D11_BUFFER_DESC cbd2{};
    cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd2.Usage = D3D11_USAGE_DEFAULT;
    cbd2.CPUAccessFlags = 0u;
    cbd2.MiscFlags = 0u;
    cbd2.ByteWidth = sizeof(cb2);
    cbd2.StructureByteStride = 0u;

    D3D11_SUBRESOURCE_DATA data3{};
    data3.pSysMem = &cb2;

    GFX_THROW_INFO(pDevice->CreateBuffer(&cbd2, &data3, &p_const_buffer2));

    pContext->PSSetConstantBuffers(0u, 1u, p_const_buffer2.GetAddressOf());

    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    WRL::ComPtr<ID3D11VertexShader> p_vertex_shader{};
    WRL::ComPtr<ID3DBlob> p_blob{};
    GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &p_blob));
    GFX_THROW_INFO(
        pDevice->CreateVertexShader(p_blob->GetBufferPointer(), p_blob->GetBufferSize(), nullptr, &p_vertex_shader));
    pContext->VSSetShader(p_vertex_shader.Get(), nullptr, 0u);

    WRL::ComPtr<ID3D11InputLayout> p_input_layout{};
    const D3D11_INPUT_ELEMENT_DESC ied[]{
        { "POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u }
    };
    GFX_THROW_INFO(pDevice->CreateInputLayout(
        ied, (UINT)std::size(ied), p_blob->GetBufferPointer(), p_blob->GetBufferSize(), &p_input_layout));
    pContext->IASetInputLayout(p_input_layout.Get());

    WRL::ComPtr<ID3D11PixelShader> p_pixel_shader{};
    GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &p_blob));
    GFX_THROW_INFO(
        pDevice->CreatePixelShader(p_blob->GetBufferPointer(), p_blob->GetBufferSize(), nullptr, &p_pixel_shader));
    pContext->PSSetShader(p_pixel_shader.Get(), nullptr, 0u);

    D3D11_VIEWPORT vp[]{ { 0.0f, 0.0f, (FLOAT)ScreenWidth, (FLOAT)ScreenHeight, 0.0f, 1.0f } };

    pContext->RSSetViewports(1u, &vp[0]);

    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDsv.Get());
    GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
    : Exception(line, file)
{
    for (const auto& m : infoMsgs)
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
    oss << GetType() << "\n\n[Error Info]\n" << GetErrorInfo() << "\n\n";
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

auto Graphics::InfoException::GetType() const noexcept -> const char*
{
    return "Mikastiv Graphics Info Exception";
}

auto Graphics::InfoException::GetErrorInfo() const noexcept -> std::string
{
    return info;
}
