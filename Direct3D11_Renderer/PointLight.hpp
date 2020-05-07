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

private:
    struct PointLightCBuf
    {
        DirectX::XMFLOAT3A pos;
        DirectX::XMFLOAT3A materialColor;
        DirectX::XMFLOAT3A ambient;
        DirectX::XMFLOAT3 diffuseColor;
        float diffuseIntensity;
        float attConst;
        float attLinear;
        float attQuad;
    };

private:
    PointLightCBuf cbData;
    mutable SolidSphere mesh;
    mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};