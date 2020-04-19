#include "App.hpp"

App::App()
    : wnd(Graphics::ScreenWidth, Graphics::ScreenHeight, windowTitle)
{
}

auto App::DoFrame() -> void
{
    wnd.GetGfx().ClearBuffer(0.0f, 0.0f, 0.0f);
    wnd.GetGfx().DrawTestTriangle(ft.Peek(), wnd.mouse.GetPosX(), wnd.mouse.GetPosY());
    wnd.GetGfx().EndFrame();
}

auto App::Start() -> int
{
    if (!wnd.HasGfx())
    {
        wnd.CreateGfx();
    }

    while (true)
    {
        if (const auto code = Window::ProcessMessages())
        {
            return code.value();
        }
        DoFrame();
    }
}
