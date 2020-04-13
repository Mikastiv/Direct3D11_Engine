#include "App.hpp"

App::App()
    : wnd(Graphics::screen_width, Graphics::screen_height, window_title)
{
}

auto App::do_frame() -> void
{
    wnd.gfx().clear_buffer(0.0f, 0.0f, 0.0f);
    wnd.gfx().draw_test_triangle();
    wnd.gfx().end_frame();
}

auto App::start() -> int
{
    if (!wnd.has_gfx())
    {
        wnd.create_gfx();
    }

    while (true)
    {
        if (const auto code = Window::process_messages())
        {
            return code.value();
        }
        do_frame();
    }
}
