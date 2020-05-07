#pragma once

#include "ConstantBuffers.hpp"
#include "Drawable.hpp"

#include <DirectXMath.h>

class TransformConstBuffer : public Bindable
{
private:
    struct Transforms
    {
        DirectX::XMMATRIX model;
        DirectX::XMMATRIX modelViewProj;
    };

public:
    TransformConstBuffer(Graphics& gfx, const Drawable& parent, UINT slot = 0u);
    auto Bind(Graphics& gfx) noexcept -> void override;

private:
    inline static std::unique_ptr<VertexConstantBuffer<Transforms>> pVertexCBuffer{};
    const Drawable& parent;
};