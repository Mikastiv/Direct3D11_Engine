#pragma once

#include "Drawable.hpp"

class Box : public Drawable
{
public:
    explicit Box(Graphics& gfx);
    auto Update(float deltaTime) noexcept -> void override;
    auto GetTransformXM() const noexcept -> DirectX::XMMATRIX override;
};