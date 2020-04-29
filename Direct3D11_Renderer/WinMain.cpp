#include "App.hpp"
#include "Helpers.hpp"

#include <clocale>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    setlocale(LC_ALL, "en-US");

    try
    {
        return App{}.Start();
    }
    catch (const MikastivException& e)
    {
        MessageBox(
            nullptr, CharToWString(e.what()).c_str(), CharToWString(e.GetType()).c_str(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBox(nullptr, CharToWString(e.what()).c_str(), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return -1;
}