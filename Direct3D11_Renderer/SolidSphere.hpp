#pragma once

#include "Entity.hpp"

class SolidSphere : public Entity<SolidSphere>
{
public:
    SolidSphere(Graphics& gfx, float radius);
    auto Update(float deltaTime) noexcept -> void override;
    auto SetPos(DirectX::XMFLOAT3 pos) noexcept -> void;
    auto GetTransformXM() const noexcept -> DirectX::XMMATRIX override;

private:
    DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
};