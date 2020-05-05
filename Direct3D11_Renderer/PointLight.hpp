#pragma once

#include "Graphics.hpp"
#include "SolidSphere.hpp"
#include "ConstantBuffers.hpp"

class PointLight
{
public:
    PointLight(Graphics& gfx, float radius = 0.5f);
    auto ShowControlWindow() noexcept -> void;
    auto Reset() noexcept -> void;
    auto Draw(Graphics& gfx) const noexcept(!IS_DEBUG) -> void;
    auto Bind(Graphics& gfx) const noexcept -> void;
    auto SetPos(DirectX::XMFLOAT3 pos) -> void;
    auto GetPos() const -> DirectX::XMFLOAT3;

private:
    struct PointLightCBuf
    {
        DirectX::XMFLOAT3 pos;
        float padding;
    };

private:
    DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
    mutable SolidSphere mesh;
    mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};