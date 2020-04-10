#include "App.hpp"
#include "Helpers.hpp"

#include <clocale>

auto CALLBACK WinMain([[maybe_unused]] HINSTANCE hInstance,
                      [[maybe_unused]] HINSTANCE hPrevInstance,
                      [[maybe_unused]] LPSTR lpCmdLine,
                      [[maybe_unused]] int nShowCmd) -> int
{
    setlocale(LC_ALL, "en-US");

    try
    {
        App app{};
        return app.start();
    }
    catch (const MikastivException& e)
    {
        MessageBox(
            nullptr, char_to_wstr(e.what()).c_str(), char_to_wstr(e.get_type()).c_str(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBox(nullptr, char_to_wstr(e.what()).c_str(), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return -1;
}