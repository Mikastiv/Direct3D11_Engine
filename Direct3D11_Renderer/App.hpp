#pragma once

#include "Window.hpp"
#include "MikastivTimer.hpp"

class App
{
public:
    App();
    auto Start() -> int;

private:
    auto DoFrame() -> void;

private:
    static constexpr const wchar_t* windowTitle = L"Direct3D 11 Engine";

private:
    Window wnd;
    Timer ft{};
};
