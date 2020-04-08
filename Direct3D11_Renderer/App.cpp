#include "App.hpp"

App::App()
    : wnd(window_width, window_height, window_title)
{
}

auto App::do_frame() -> void
{

}

auto App::start() -> int
{
    while (true)
    {
        if (const auto code = Window::process_messages())
        {
            return code.value();
        }
        do_frame();
    }
}
