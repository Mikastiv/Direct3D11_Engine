#pragma once

#include "Entity.hpp"

#include <random>

template <typename T>
class TestObject : public Entity<T>
{
public:
    TestObject(std::mt19937& rng,
               std::uniform_real_distribution<float>& radiusDist,
               std::uniform_real_distribution<float>& yRotOffsetDist,
               std::uniform_real_distribution<float>& rotOffsetDist,
               std::uniform_real_distribution<float>& dYRotationDist,
               std::uniform_real_distribution<float>& dPitchDist,
               std::uniform_real_distribution<float>& dYawDist,
               std::uniform_real_distribution<float>& dRollDist)
        : rotationRadius(radiusDist(rng))
        , rotationOffset(rotOffsetDist(rng))
        , yRotation(yRotOffsetDist(rng))
        , dYRotation(dYRotationDist(rng))
        , dPitch(dPitchDist(rng))
        , dYaw(dYawDist(rng))
        , dRoll(dRollDist(rng))
    {
    }
    auto Update(float deltaTime) noexcept -> void override
    {
        yRotation += dYRotation * deltaTime;
        pitch += dPitch * deltaTime;
        yaw += dYaw * deltaTime;
        roll += dRoll * deltaTime;
    }
    auto GetTransformXM() const noexcept -> DirectX::XMMATRIX override
    {
        return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
               DirectX::XMMatrixTranslation(rotationRadius, 0.0f, 0.0f) *
               DirectX::XMMatrixRotationRollPitchYaw(rotationOffset, yRotation, rotationOffset) *
               DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
    }

private:
    const float rotationRadius = 0.0f;
    const float rotationOffset = 0.0f;
    float yRotation = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;

    float dYRotation = 0.0f;
    float dPitch = 0.0f;
    float dYaw = 0.0f;
    float dRoll = 0.0f;
};