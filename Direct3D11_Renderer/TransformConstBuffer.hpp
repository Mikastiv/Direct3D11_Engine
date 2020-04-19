#pragma once

#include "ConstantBuffers.hpp"
#include "Drawable.hpp"

#include <DirectXMath.h>

class TransformConstBuffer : public Bindable
{
public:
    TransformConstBuffer(Graphics& gfx, const Drawable& parent);
    auto Bind(Graphics& gfx) noexcept -> void override;

private:
    VertexConstantBuffer<DirectX::XMMATRIX> vertexCBuffer;
    const Drawable& parent;
};