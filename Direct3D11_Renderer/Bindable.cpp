#include "Bindable.hpp"
#include "GraphicsMacros.hpp"

#include <stdexcept>

auto Bindable::GetContext(Graphics& gfx) noexcept -> ID3D11DeviceContext*
{
    return gfx.pContext.Get();
}

auto Bindable::GetDevice(Graphics& gfx) noexcept -> ID3D11Device*
{
    return gfx.pDevice.Get();
}

auto Bindable::GetInfoManager(Graphics& gfx) noexcept(!IS_DEBUG) -> DXGIInfoManager&
{
#ifdef NDEBUG
    throw std::logic_error("Tried to access gfx.info_manager in Release config");
#else
    return gfx.infoManager;
#endif
}
