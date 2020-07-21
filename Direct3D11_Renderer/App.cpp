#include "App.hpp"
#include "GDIPlusManager.hpp"
#include "Box.hpp"
#include "imgui/imgui.h"

#include <random>
#include <algorithm>

GDIPlusManager gdiManager;

App::App()
    : wnd(Graphics::ScreenWidth, Graphics::ScreenHeight, windowTitle)
    , gfx(wnd.GetHWND())
    , light(gfx)
{
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> radiusDist(5.0f, 20.0f);
    std::uniform_real_distribution<float> yRotDist(0.0f, DirectX::XM_2PI);
    std::uniform_real_distribution<float> rotOffsetDist(-DirectX::XM_PIDIV4, DirectX::XM_PIDIV4);
    std::uniform_real_distribution<float> dYRotDist(0.01f, DirectX::XM_PIDIV4 / 2.0f);
    std::uniform_real_distribution<float> modelRotDist(0.1f, DirectX::XM_PIDIV2);
    std::uniform_int_distribution<int> typeDist(0, 1);
    std::uniform_real_distribution<float> matColorDist(0.0f, 1.0f);

    const auto GenerateTestObjects = [&]() -> std::unique_ptr<Drawable> {
        const DirectX::XMFLOAT3 color = { matColorDist(rng), matColorDist(rng), matColorDist(rng) };
        switch (typeDist(rng))
        {
        default:
            return std::make_unique<Box>(
                gfx,
                rng,
                radiusDist,
                yRotDist,
                rotOffsetDist,
                dYRotDist,
                modelRotDist,
                modelRotDist,
                modelRotDist,
                color);
        }
    };

    std::generate_n(std::back_inserter(drawables), nDrawables, GenerateTestObjects);

    constexpr auto ar = (float)Graphics::ScreenWidth / (float)Graphics::ScreenHeight;
    constexpr auto fovy = DirectX::XMConvertToRadians(60.0f);
    gfx.SetProjection(DirectX::XMMatrixPerspectiveFovLH(fovy, ar, 0.5f, 60.0f));
}

auto App::DoFrame() -> void
{
    const float deltaTime = ft.Mark() * speedFactor;
    gfx.BeginFrame(0.0f, 0.0f, 0.0f);
    gfx.SetCameraView(camera.GetViewMatrix());
    light.Bind(gfx, camera.GetViewMatrix());

    for (auto& d : drawables)
    {
        d->Update(deltaTime);
        d->Draw(gfx);
    }
    light.Draw(gfx);

    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    ImGui::SetNextWindowSize({ 170.0f, 90.0f });
    if (ImGui::Begin("Simulation Speed"))
    {
        ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        ImGui::SliderFloat("Speed", &speedFactor, 0.0f, 5.0f, "%.2f");
    }
    ImGui::End();

    camera.ShowControlWindow();
    light.ShowControlWindow();

    gfx.EndFrame();
}

auto App::Start() -> int
{
    while (true)
    {
        if (const auto code = Window::ProcessMessages())
        {
            return code.value();
        }
        DoFrame();
    }
}
