#pragma once

#include "Window.hpp"
#include "MikastivTimer.hpp"
#include "Box.hpp"
#include "Sheet.hpp"
#include "SkinnedBox.hpp"

class App
{
public:
    App();
    App(const App&) = delete;
    App(App&&) = delete;
    ~App() = default;
    auto operator=(const App&) -> App& = delete;
    auto operator=(App&&) -> App& = delete;
    auto Start() -> int;

private:
    auto DoFrame() -> void;

private:
    static constexpr const wchar_t* windowTitle = L"Direct3D 11 Engine";

private:
    Window wnd;
    Timer ft{};
    std::vector<std::unique_ptr<Box>> boxes{};
    std::vector<std::unique_ptr<Sheet>> sheets{};
    std::vector<std::unique_ptr<SkinnedBox>> sboxes{};
};
