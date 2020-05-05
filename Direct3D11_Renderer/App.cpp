#include "App.hpp"
#include "GDIPlusManager.hpp"
#include "SkinnedBox.hpp"
#include "Globe.hpp"
#include "imgui/imgui.h"

#include <random>
#include <algorithm>

GDIPlusManager gdiManager;

App::App()
    : wnd(Graphics::ScreenWidth, Graphics::ScreenHeight, windowTitle)
{
    if (!wnd.HasGfx())
    {
        wnd.CreateGfx();
    }

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> radiusDist(5.0f, 20.0f);
    std::uniform_real_distribution<float> yRotDist(0.0f, DirectX::XM_2PI);
    std::uniform_real_distribution<float> rotOffsetDist(-DirectX::XM_PIDIV4, DirectX::XM_PIDIV4);
    std::uniform_real_distribution<float> dYRotDist(0.01f, DirectX::XM_PIDIV4 / 2.0f);
    std::uniform_real_distribution<float> modelRotDist(0.1f, DirectX::XM_PIDIV2);
    std::uniform_int_distribution<int> typeDist(0, 1);

    const auto GenerateTestObjects = [&]() -> std::unique_ptr<Drawable> {
        switch (typeDist(rng))
        {
        case 0:
            return std::make_unique<Globe>(
                wnd.GetGfx(),
                rng,
                radiusDist,
                yRotDist,
                rotOffsetDist,
                dYRotDist,
                modelRotDist,
                modelRotDist,
                modelRotDist);
        default:
            return std::make_unique<SkinnedBox>(
                wnd.GetGfx(),
                rng,
                radiusDist,
                yRotDist,
                rotOffsetDist,
                dYRotDist,
                modelRotDist,
                modelRotDist,
                modelRotDist);
        }
    };

    std::generate_n(std::back_inserter(drawables), nDrawables, GenerateTestObjects);

    const auto ar = (float)Graphics::ScreenHeight / (float)Graphics::ScreenWidth;
    wnd.GetGfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, ar, 0.5f, 60.0f));
}

auto App::DoFrame() -> void
{
    const float deltaTime = ft.Mark() * speedFactor;
    wnd.GetGfx().BeginFrame(0.0f, 0.0f, 0.0f);
    wnd.GetGfx().SetCameraView(camera.GetViewMatrix());
    for (auto& d : drawables)
    {
        d->Update(deltaTime);
        d->Draw(wnd.GetGfx());
    }

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

    wnd.GetGfx().EndFrame();
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
