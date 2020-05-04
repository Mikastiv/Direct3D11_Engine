#include "Graphics.hpp"
#include "Helpers.hpp"
#include "GraphicsMacros.hpp"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <array>

namespace WRL = Microsoft::WRL;

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

    D3D11_VIEWPORT vp[]{ { 0.0f, 0.0f, (FLOAT)ScreenWidth, (FLOAT)ScreenHeight, 0.0f, 1.0f } };
    pContext->RSSetViewports(1u, &vp[0]);

    ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
}

Graphics::~Graphics()
{
    ImGui_ImplDX11_Shutdown();
}

auto Graphics::EndFrame() -> void
{
    if (imguiEnabled)
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    HRESULT hr{};
#ifndef NDEBUG
    infoManager.Set();
#endif

    if (FAILED(hr = pSwap->Present(0u, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        throw GFX_EXCEPT(hr);
    }
}

auto Graphics::BeginFrame(float red, float green, float blue) noexcept -> void
{
    if (imguiEnabled)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    const std::array<float, 4> color = { red, green, blue, 1.0f };
    pContext->ClearRenderTargetView(pTarget.Get(), color.data());
    pContext->ClearDepthStencilView(pDsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

auto Graphics::DrawIndexed(UINT count) noexcept(!IS_DEBUG) -> void
{
    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDsv.Get());
    GFX_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0u, 0u));
}

auto Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept -> void
{
    projection = proj;
}

auto Graphics::GetProjection() const noexcept -> DirectX::XMMATRIX
{
    return projection;
}

auto Graphics::SetCameraView(DirectX::FXMMATRIX camera) noexcept -> void
{
    cameraView = camera;
}

auto Graphics::GetCameraView() const noexcept -> DirectX::XMMATRIX
{
    return cameraView;
}

auto Graphics::EnableImGui() noexcept -> void
{
    imguiEnabled = true;
}

auto Graphics::DisableImGui() noexcept -> void
{
    imguiEnabled = false;
}

auto Graphics::IsImGuiEnabled() const noexcept -> bool
{
    return imguiEnabled;
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
