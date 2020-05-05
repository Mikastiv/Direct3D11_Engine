#pragma once

#include <DirectXMath.h>

class Camera
{
public:
    auto GetViewMatrix() const noexcept -> DirectX::XMMATRIX;
    auto ShowControlWindow() noexcept -> void;
    auto Reset() noexcept -> void;

private:
    float r = 20.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;
    float xRotation = 0.0f;
    float yRotation = 0.0f;
};
