#include "DXGIInfoManager.hpp"
#include "Window.hpp"

#include <dxgidebug.h>

#pragma comment(lib, "dxguid.lib")

#define GFX_THROW_NOINFO(hrcall)                                                                                       \
    if (FAILED(hr = (hrcall)))                                                                                         \
    throw Graphics::HrException(__LINE__, __FILE__, hr)

DXGIInfoManager::DXGIInfoManager()
{
    // define function signature of DXGIGetDebugInterface
    typedef HRESULT(WINAPI * DXGIGetDebugInterface)(REFIID, void**);

    // load the dll that contains the function DXGIGetDebugInterface
    const auto h_mod_dxgi_debug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (h_mod_dxgi_debug == nullptr)
    {
        throw MKWND_LAST_EXCEPT();
    }

    // get address of DXGIGetDebugInterface in dll
    const auto dxgi_get_debug_interface = reinterpret_cast<DXGIGetDebugInterface>(
        reinterpret_cast<void*>(GetProcAddress(h_mod_dxgi_debug, "DXGIGetDebugInterface")));

    if (dxgi_get_debug_interface == nullptr)
    {
        throw MKWND_LAST_EXCEPT();
    }

    HRESULT hr{};
    GFX_THROW_NOINFO(dxgi_get_debug_interface(__uuidof(IDXGIInfoQueue), &p_dxgi_info_queue));
}

auto DXGIInfoManager::set() noexcept -> void
{
    // set the index (next) so that the next all to GetMessages()
    // will only get errors generated after this call
    next = p_dxgi_info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

auto DXGIInfoManager::get_messages() const -> std::vector<std::string>
{
    std::vector<std::string> messages;
    const auto end = p_dxgi_info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL);
    for (auto i = next; i < end; i++)
    {
        HRESULT hr{};
        SIZE_T message_length{};
        // get the size of message i in bytes
        GFX_THROW_NOINFO(p_dxgi_info_queue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &message_length));

        // allocate memory for message
        auto bytes = std::make_unique<byte[]>(message_length);
        auto p_message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

        // get the message and push its description into the vector
        GFX_THROW_NOINFO(p_dxgi_info_queue->GetMessage(DXGI_DEBUG_ALL, i, p_message, &message_length));
        messages.emplace_back(p_message->pDescription);
    }
    return messages;
}
