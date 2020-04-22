#pragma once

#include "Entity.hpp"

class Box : public Entity<Box>
{
public:
    Box(Graphics& gfx, float xOffset);
    auto Update(float deltaTime) noexcept -> void override;
    auto GetTransformXM() const noexcept -> DirectX::XMMATRIX override;

private:
    float xOffset = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;
    float dPitch = 0.5f;
    float dYaw = 0.2f;
    float dRoll = 0.6f;
};