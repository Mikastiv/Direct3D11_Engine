#pragma once

#include "Graphics.hpp"

class Bindable
{
public:
    Bindable() = default;
    Bindable(const Bindable&) = delete;
    Bindable(Bindable&&) = delete;
    virtual ~Bindable() = default;
    auto operator=(const Bindable&) -> Bindable& = delete;
    auto operator=(Bindable &&) -> Bindable& = delete;
    virtual auto Bind(Graphics& gfx) noexcept -> void = 0;

protected:
    static auto GetContext(Graphics& gfx) noexcept -> ID3D11DeviceContext*;
    static auto GetDevice(Graphics& gfx) noexcept -> ID3D11Device*;
    static auto GetInfoManager(Graphics& gfx) noexcept(IS_DEBUG) -> DXGIInfoManager&;
};