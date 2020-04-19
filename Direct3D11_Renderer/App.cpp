#include "App.hpp"

App::App()
    : wnd(Graphics::ScreenWidth, Graphics::ScreenHeight, windowTitle)
{
    if (!wnd.HasGfx())
    {
        wnd.CreateGfx();
    }
    boxes.push_back(std::make_unique<Box>(wnd.GetGfx()));
    const auto ar = (float)Graphics::ScreenHeight / (float)Graphics::ScreenWidth;
    wnd.GetGfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, ar, 0.5f, 40.0f));
}

auto App::DoFrame() -> void
{
    const float deltaTime = ft.Mark();
    wnd.GetGfx().ClearBuffer(0.0f, 0.0f, 0.0f);
    for (auto& box : boxes)
    {
        box->Update(deltaTime);
        box->Draw(wnd.GetGfx());
    }
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
