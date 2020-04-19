#pragma once

#include "WinDefines.hpp"

#include <wrl.h>
#include <dxgidebug.h>
#include <string>
#include <vector>

class DXGIInfoManager
{
public:
    DXGIInfoManager();
    DXGIInfoManager(const DXGIInfoManager&) = delete;
    DXGIInfoManager(DXGIInfoManager&&) = delete;
    ~DXGIInfoManager() = default;
    auto operator=(const DXGIInfoManager&) -> DXGIInfoManager& = delete;
    auto operator=(DXGIInfoManager &&) -> DXGIInfoManager& = delete;
    auto Set() noexcept -> void;
    auto GetMessages() const -> std::vector<std::string>;

private:
    uint64_t next = 0u;
    Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDXGIInfoQueue{};
};