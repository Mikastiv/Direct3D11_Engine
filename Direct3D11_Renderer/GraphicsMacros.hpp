#pragma once

// HRESULT hr should exist in the local scope for these macros to work

#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException(__LINE__, __FILE__, hr)

#define GFX_THROW_NOINFO(hrcall)                                                                                       \
    if (FAILED(hr = (hrcall)))                                                                                         \
    throw Graphics::HrException(__LINE__, __FILE__, hr)

#define GFX_THROW_NOINFO(hrcall)                                                                                       \
    if (FAILED(hr = (hrcall)))                                                                                         \
    throw Graphics::HrException(__LINE__, __FILE__, hr)

#ifdef _DEBUG
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__, __FILE__, hr, info_manager.get_messages())
#define GFX_THROW_INFO(hrcall)                                                                                         \
    info_manager.set();                                                                                                \
    if (FAILED(hr = (hrcall)))                                                                                         \
    throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr)                                                                                  \
    Graphics::DeviceRemovedException(__LINE__, __FILE__, hr, info_manager.get_messages())
#define GFX_THROW_INFO_ONLY(fn_call)                                                                                   \
    info_manager.set();                                                                                                \
    fn_call;                                                                                                           \
    {                                                                                                                  \
        auto v = info_manager.get_messages();                                                                          \
        if (!v.empty())                                                                                                \
        {                                                                                                              \
            throw Graphics::InfoException(__LINE__, __FILE__, v);                                                      \
        }                                                                                                              \
    }
#else
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__, __FILE__, hr)
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)
#define GFX_THROW_INFO_ONLY(fn_call) fn_call
#endif

// macro for importing infomanager into local scope
// this.GetInfoManager(Graphics& gfx) must exist
#ifdef _DEBUG
#define INFOMAN(gfx)                                                                                                   \
    HRESULT hr;                                                                                                        \
    DXGIInfoManager& info_manager = GetInfoManager(gfx)
#else
#define INFOMAN(gfx) HRESULT hr
#endif