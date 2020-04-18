#pragma once

#define MKWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define MKWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())
#define MKWND_NOGFX_EXCEPT() Window::NoGfxException(__LINE__, __FILE__)