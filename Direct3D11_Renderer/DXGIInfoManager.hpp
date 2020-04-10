#pragma once

#include "WinDefines.hpp"

#include <string>
#include <vector>

class DXGIInfoManager
{
private:
    uint64_t next = 0u;
    struct IDXGIInfoQueue* p_dxgi_info_queue = nullptr;

public:
    DXGIInfoManager();
    DXGIInfoManager(const DXGIInfoManager&) = delete;
    DXGIInfoManager(DXGIInfoManager&&) = delete;
    ~DXGIInfoManager();
    auto operator=(const DXGIInfoManager&) -> DXGIInfoManager& = delete;
    auto operator=(DXGIInfoManager &&) -> DXGIInfoManager& = delete;
    auto set() noexcept -> void;
    auto get_messages() const -> std::vector<std::string>;

};