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
    inline static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVertexCBuffer{};
    const Drawable& parent;
};