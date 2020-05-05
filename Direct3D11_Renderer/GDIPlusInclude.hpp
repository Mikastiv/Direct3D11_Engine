#pragma once

// Enables windows stuff for gdi plus
#define ENABLE_WIN_MACROS
#include "WinDefines.hpp"

#include <algorithm>
namespace Gdiplus
{
    using std::max;
    using std::min;
}
#include <gdiplus.h>