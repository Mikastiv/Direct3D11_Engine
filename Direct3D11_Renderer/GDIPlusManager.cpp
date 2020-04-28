#include "GDIPlusInclude.hpp"
#include "GDIPlusManager.hpp"

#pragma comment(lib, "gdiplus.lib")

GDIPlusManager::GDIPlusManager()
{
    if (RefCount++ == 0)
    {
        Gdiplus::GdiplusStartupInput input;
        input.GdiplusVersion = 1;
        input.DebugEventCallback = nullptr;
        Gdiplus::GdiplusStartup(&Token, &input, nullptr);
    }
}

GDIPlusManager::~GDIPlusManager()
{
    if (--RefCount == 0)
    {
        Gdiplus::GdiplusShutdown(Token);
    }
}
