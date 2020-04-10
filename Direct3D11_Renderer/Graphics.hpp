#pragma once

#include "WinDefines.hpp"
#include "MikastivException.hpp"
#include "DXGIInfoManager.hpp"

#include <d3d11.h>
#include <wrl.h>
#include <vector>

class Graphics
{
public:
    class Exception : public MikastivException
    {
    private:
        HRESULT hr{};
        std::string info{};

    public:
        Exception(int line, const char* file, HRESULT hr, std::vector<std::string> info_msgs = {}) noexcept;
        auto what() const noexcept -> const char* override;
        auto get_type() const noexcept -> const char* override;
        auto get_error_code() const noexcept -> HRESULT;
        auto get_error_description() const noexcept -> std::string;
        auto get_error_info() const noexcept -> std::string;
    };

    class DeviceRemovedException : public Exception
    {
    public:
        DeviceRemovedException(int line,
                               const char* file,
                               HRESULT hr,
                               std::vector<std::string> info_msgs = {}) noexcept;
        auto get_type() const noexcept -> const char* override;
    };

private:
#ifdef _DEBUG
    DXGIInfoManager info_manager{};
#endif
    Microsoft::WRL::ComPtr<ID3D11Device> p_device{};
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> p_context{};
    Microsoft::WRL::ComPtr<IDXGISwapChain> p_swap{};
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> p_target{};

public:
    explicit Graphics(HWND h_wnd);
    Graphics(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;
    ~Graphics() = default;
    auto operator=(const Graphics&) -> Graphics& = delete;
    auto operator=(Graphics &&) -> Graphics& = delete;
    auto end_frame() -> void;
    auto clear_buffer(float red, float green, float blue) noexcept -> void;
};