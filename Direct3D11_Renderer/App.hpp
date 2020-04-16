#pragma once

#include "Window.hpp"
#include "MikastivTimer.hpp"

class App
{
public:
    App();
    auto start() -> int;

private:
    auto do_frame() -> void;

private:
    static constexpr const wchar_t* window_title = L"Direct3D 11 Engine";

private:
    Window wnd;
    Timer ft{};
};
