#pragma once

#include "Drawable.hpp"

class Box : public Drawable
{
public:
    explicit Box(Graphics& gfx);
    auto Update(float deltaTime) noexcept -> void override;
    auto GetTransformXM() const noexcept -> DirectX::XMMATRIX override;

private:
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;
    float dPitch = 0.5f;
    float dYaw = 0.2f;
    float dRoll = 0.6f;
};