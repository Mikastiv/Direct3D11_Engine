#pragma once

#include "WinDefines.hpp"

#include <wrl.h>
#include <dxgidebug.h>
#include <string>
#include <vector>

class DXGIInfoManager
{
private:
    uint64_t next = 0u;
    Microsoft::WRL::ComPtr<IDXGIInfoQueue> p_dxgi_info_queue{};

public:
    DXGIInfoManager();
    DXGIInfoManager(const DXGIInfoManager&) = delete;
    DXGIInfoManager(DXGIInfoManager&&) = delete;
    ~DXGIInfoManager() = default;
    auto operator=(const DXGIInfoManager&) -> DXGIInfoManager& = delete;
    auto operator=(DXGIInfoManager &&) -> DXGIInfoManager& = delete;
    auto set() noexcept -> void;
    auto get_messages() const -> std::vector<std::string>;
};