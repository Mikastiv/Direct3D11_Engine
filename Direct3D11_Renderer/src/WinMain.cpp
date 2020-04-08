#include "include/WinDefines.hpp"
#include "include/Window.hpp"
#include "include/Helpers.hpp"

#include <locale.h>

auto CALLBACK WinMain([[maybe_unused]] HINSTANCE hInstance,
                      [[maybe_unused]] HINSTANCE hPrevInstance,
                      [[maybe_unused]] LPSTR lpCmdLine,
                      [[maybe_unused]] int nShowCmd) -> int
{
    setlocale(LC_ALL, "en_US");

    constexpr int wnd_width = 800;
    constexpr int wnd_height = 600;

    try
    {
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
    catch (const MikastivException& e)
    {
        std::string ex_what{ e.what() };
        std::string ex_type{ e.get_type() };
        MessageBox(nullptr,
                   char_to_wstr(ex_what.data()).c_str(),
                   char_to_wstr(ex_type.data()).c_str(),
                   MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        std::string ex_what{ e.what() };
        MessageBox(nullptr, char_to_wstr(ex_what.data()).c_str(), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return -1;
}