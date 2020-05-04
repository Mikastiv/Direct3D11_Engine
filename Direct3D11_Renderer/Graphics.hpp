#pragma once

#include "WinDefines.hpp"
#include "MikastivException.hpp"
#include "DXGIInfoManager.hpp"
#include "DebugMacro.hpp"

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>

class Graphics
{
    friend class Bindable;

public:
    class Exception : public MikastivException
    {
        using MikastivException::MikastivException;
    };

    class HrException : public Exception
    {
    public:
        HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
        auto what() const noexcept -> const char* override;
        auto GetType() const noexcept -> const char* override;
        auto GetErrorCode() const noexcept -> HRESULT;
        auto GetErrorDescription() const noexcept -> std::string;
        auto GetErrorInfo() const noexcept -> std::string;

    private:
        HRESULT hr{};
        std::string info{};
    };

    class InfoException : public Exception
    {
    public:
        InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
        auto what() const noexcept -> const char* override;
        auto GetType() const noexcept -> const char* override;
        auto GetErrorInfo() const noexcept -> std::string;

    private:
        std::string info{};
    };

    class DeviceRemovedException : public HrException
    {
    public:
        DeviceRemovedException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
        auto GetType() const noexcept -> const char* override;
    };

public:
    explicit Graphics(HWND hWnd);
    Graphics(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;
    ~Graphics();
    auto operator=(const Graphics&) -> Graphics& = delete;
    auto operator=(Graphics &&) -> Graphics& = delete;
    auto EndFrame() -> void;
    auto BeginFrame(float red, float green, float blue) noexcept -> void;
    auto DrawIndexed(UINT count) noexcept(!IS_DEBUG) -> void;
    auto SetProjection(DirectX::FXMMATRIX proj) noexcept -> void;
    auto GetProjection() const noexcept -> DirectX::XMMATRIX;
    auto SetCameraView(DirectX::FXMMATRIX camera) noexcept -> void;
    auto GetCameraView() const noexcept -> DirectX::XMMATRIX;
    auto EnableImGui() noexcept -> void;
    auto DisableImGui() noexcept -> void;
    auto IsImGuiEnabled() const noexcept -> bool;

public:
    static constexpr int ScreenWidth = 1280;
    static constexpr int ScreenHeight = 720;

private:
    bool imguiEnabled = true;
    DirectX::XMMATRIX projection{};
    DirectX::XMMATRIX cameraView{};
#ifndef NDEBUG
    DXGIInfoManager infoManager{};
#endif
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice{};
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext{};
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap{};
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget{};
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDsv{};
};