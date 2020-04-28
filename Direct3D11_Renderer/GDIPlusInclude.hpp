#pragma once

// Enables windows stuff for gdi plus
#define ENABLE_WIN_MACROS
#include "WinDefines.hpp"

#include <algorithm>
namespace Gdiplus
{
    using std::min;
    using std::max;
}
#include <gdiplus.h>