#include "Camera.hpp"
#include "imgui/imgui.h"

auto Camera::GetViewMatrix() const noexcept -> DirectX::XMMATRIX
{
    const auto pos = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
                                                 DirectX::XMMatrixRotationRollPitchYaw(xRotation, -yRotation, 0.0f));
    return DirectX::XMMatrixLookAtLH(pos, DirectX::XMVectorZero(), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) *
           DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
}

auto Camera::ShowControlWindow() noexcept -> void
{
    if (ImGui::Begin("Camera"))
    {
        ImGui::Text("Position");
        ImGui::SliderFloat("R", &r, 0.001f, 80.0f, "%.3f");
        ImGui::SliderAngle("X Rotation", &xRotation, -89.0f, 89.0f);
        ImGui::SliderAngle("Y Rotation", &yRotation, -180.0f, 180.0f);
        ImGui::Text("Orientation");
        ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
        ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
        ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
        if (ImGui::Button("Reset"))
        {
            Reset();
        }
    }
    ImGui::End();
}

auto Camera::Reset() noexcept -> void
{
    r = 20.0f;
    pitch = 0.0f;
    yaw = 0.0f;
    roll = 0.0f;
    xRotation = 0.0f;
    yRotation = 0.0f;
}
