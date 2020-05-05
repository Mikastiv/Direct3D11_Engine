#pragma once

// HRESULT hr should exist in the local scope for these macros to work

#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException(__LINE__, __FILE__, hr)

#define GFX_THROW_NOINFO(hrcall)                                                                                       \
    if (FAILED(hr = (hrcall)))                                                                                         \
    throw Graphics::HrException(__LINE__, __FILE__, hr)

#define GFX_THROW_NOINFO(hrcall)                                                                                       \
    if (FAILED(hr = (hrcall)))                                                                                         \
    throw Graphics::HrException(__LINE__, __FILE__, hr)

#ifdef NDEBUG
#define GFX_EXCEPT(hr)                Graphics::HrException(__LINE__, __FILE__, hr)
#define GFX_THROW_INFO(hrcall)        GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)
#define GFX_THROW_INFO_ONLY(fn_call)  fn_call
#else
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__, __FILE__, hr, infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall)                                                                                         \
    infoManager.Set();                                                                                                 \
    if (FAILED(hr = (hrcall)))                                                                                         \
    throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr)                                                                                  \
    Graphics::DeviceRemovedException(__LINE__, __FILE__, hr, infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(fn_call)                                                                                   \
    infoManager.Set();                                                                                                 \
    fn_call;                                                                                                           \
    {                                                                                                                  \
        auto v = infoManager.GetMessages();                                                                            \
        if (!v.empty())                                                                                                \
        {                                                                                                              \
            throw Graphics::InfoException(__LINE__, __FILE__, v);                                                      \
        }                                                                                                              \
    }
#endif

// macro for importing infomanager into local scope
// this.GetInfoManager(Graphics& gfx) must exist
#ifdef NDEBUG
#define INFOMAN(gfx) HRESULT hr;
#else
#define INFOMAN(gfx)                                                                                                   \
    HRESULT hr;                                                                                                        \
    DXGIInfoManager& infoManager = GetInfoManager(gfx)
#endif