#include "include/WinDefines.hpp"
#include "include/Window.hpp"

auto CALLBACK WinMain([[maybe_unused]] HINSTANCE hInstance,
                      [[maybe_unused]] HINSTANCE hPrevInstance,
                      [[maybe_unused]] LPSTR lpCmdLine,
                      [[maybe_unused]] int nShowCmd) -> int
{
    constexpr int wnd_width = 800;
    constexpr int wnd_height = 600;
    Window wnd(wnd_width, wnd_height, L"Direct3D11 Engine");

    MSG msg{};
    BOOL msg_result{};
    while ((msg_result = GetMessage(&msg, nullptr, 0, 0)) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg_result == -1)
    {
        return -1;
    }
    return (int)msg.wParam;
}