#pragma once

#include "Window.hpp"
#include "MikastivTimer.hpp"
#include "Drawable.hpp"
#include "ImGuiManager.hpp"
#include "Camera.hpp"
#include "PointLight.hpp"

class App
{
public:
    App();
    App(const App&) = delete;
    App(App&&) = delete;
    ~App() = default;
    auto operator=(const App&) -> App& = delete;
    auto operator=(App &&) -> App& = delete;
    auto Start() -> int;

private:
    auto DoFrame() -> void;

private:
    static constexpr const wchar_t* windowTitle = L"Direct3D 11 Engine";

private:
    static constexpr uint32_t nDrawables = 200;
    bool showDemoWindow = false;
    float speedFactor = 1.0f;
    ImGuiManager imgui{};
    Window wnd;
    Timer ft{};
    Camera camera{};
    DirectX::XMFLOAT3 lightPos{};
    std::vector<std::unique_ptr<Drawable>> drawables{};
};
