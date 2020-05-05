#include "PointLight.hpp"
#include "imgui/imgui.h"

PointLight::PointLight(Graphics& gfx, float radius)
    : mesh(gfx, radius)
    , cbuf(gfx)
{
}

auto PointLight::ShowControlWindow() noexcept -> void
{
    if (ImGui::Begin("Light"))
    {
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &pos.x, -60.0f, 60.0f, "%.1f");
        ImGui::SliderFloat("Y", &pos.y, -60.0f, 60.0f, "%.1f");
        ImGui::SliderFloat("Z", &pos.z, -60.0f, 60.0f, "%.1f");
        if (ImGui::Button("Reset"))
        {
            Reset();
        }
    }
    ImGui::End();
}

auto PointLight::Reset() noexcept -> void
{
    pos = { 0.0f, 0.0f, 0.0f };
}

auto PointLight::Draw(Graphics& gfx) const noexcept(!IS_DEBUG) -> void
{
    mesh.SetPos(pos);
    mesh.Draw(gfx);
}

auto PointLight::Bind(Graphics& gfx) const noexcept -> void
{
    cbuf.Update(gfx, PointLightCBuf{ pos });
    cbuf.Bind(gfx);
}

auto PointLight::SetPos(DirectX::XMFLOAT3 pos) -> void
{
    this->pos = pos;
    mesh.SetPos(pos);
}

auto PointLight::GetPos() const -> DirectX::XMFLOAT3
{
    return pos;
}
