#pragma once

#include "Window.hpp"

class App
{
private:
    static constexpr int window_width = 1280;
    static constexpr int window_height = 720;
    static constexpr const wchar_t* window_title = L"Direct3D 11 Engine";

private:
    Window wnd;
    bool loop_game = true;

private:
    auto do_frame() -> void;

public:
    App();
    auto start() -> int;
};
