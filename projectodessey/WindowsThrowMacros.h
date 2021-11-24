#pragma once

#define ENGWND_EXCEPT(hr) Window::HrException(__LINE__, __FILE__, (hr))
#define ENGWND_LAST_EXCEPT() Window::HrException(__LINE__, __FILE__, GetLastError())
#define ENGWND_NOGFX_EXCEPT() Window::NoGfxException(__LINE__, __FILE__)
