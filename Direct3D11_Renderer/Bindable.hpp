#pragma once

#include "Graphics.hpp"
#include "GraphicsMacros.hpp"

class Bindable
{
public:
    virtual ~Bindable() = default;
    virtual auto Bind(Graphics& gfx) noexcept -> void = 0;

protected:
    static auto GetContext(Graphics& gfx) noexcept -> ID3D11DeviceContext*;
    static auto GetDevice(Graphics& gfx) noexcept -> ID3D11Device*;
    static auto GetInfoManager(Graphics& gfx) noexcept(!IS_DEBUG) -> DXGIInfoManager&;
};