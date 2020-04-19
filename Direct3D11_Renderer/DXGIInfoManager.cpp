#include "DXGIInfoManager.hpp"
#include "Window.hpp"
#include "GraphicsMacros.hpp"
#include "WindowMacros.hpp"

#include <dxgidebug.h>

#pragma comment(lib, "dxguid.lib")

DXGIInfoManager::DXGIInfoManager()
{
    // define function signature of DXGIGetDebugInterface
    typedef HRESULT(WINAPI * DXGIGetDebugInterface)(REFIID, void**);

    // load the dll that contains the function DXGIGetDebugInterface
    const auto hModDXGIDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (hModDXGIDebug == nullptr)
    {
        throw MKWND_LAST_EXCEPT();
    }

    // get address of DXGIGetDebugInterface in dll
    const auto aDXGIGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
        reinterpret_cast<void*>(GetProcAddress(hModDXGIDebug, "DXGIGetDebugInterface")));

    if (aDXGIGetDebugInterface == nullptr)
    {
        throw MKWND_LAST_EXCEPT();
    }

    HRESULT hr{};
    GFX_THROW_NOINFO(aDXGIGetDebugInterface(__uuidof(IDXGIInfoQueue), &pDXGIInfoQueue));
}

auto DXGIInfoManager::Set() noexcept -> void
{
    // set the index (next) so that the next all to GetMessages()
    // will only get errors generated after this call
    next = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

auto DXGIInfoManager::GetMessages() const -> std::vector<std::string>
{
    std::vector<std::string> messages;
    const auto end = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
    for (auto i = next; i < end; i++)
    {
        HRESULT hr{};
        SIZE_T messageLength{};
        // get the size of message i in bytes
        GFX_THROW_NOINFO(pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

        // allocate memory for message
        auto bytes = std::make_unique<byte[]>(messageLength);
        auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

        // get the message and push its description into the vector
        GFX_THROW_NOINFO(pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
        messages.emplace_back(pMessage->pDescription);
    }
    return messages;
}
