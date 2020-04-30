#pragma once

#include "Entity.hpp"

class Sheet : public Entity<Sheet>
{
public:
    explicit Sheet(Graphics& gfx);
    auto Update(float deltaTime) noexcept -> void override;
    auto GetTransformXM() const noexcept -> DirectX::XMMATRIX override;

private:
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;
    float dPitch = 0.0f;
    float dYaw = 0.0f;
    float dRoll = 0.0f;
};