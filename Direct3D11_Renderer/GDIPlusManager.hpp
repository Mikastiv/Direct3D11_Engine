#pragma once

#include "WinDefines.hpp"

class GDIPlusManager
{
public:
    GDIPlusManager();
    ~GDIPlusManager();

private:
    inline static ULONG_PTR Token = 0;
    inline static unsigned int RefCount = 0;
};