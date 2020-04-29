#include "Sampler.hpp"
#include "GraphicsMacros.hpp"

Sampler::Sampler(Graphics& gfx)
{
    INFOMAN(gfx);

    D3D11_SAMPLER_DESC desc{};
    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&desc, &pSampler));
}

auto Sampler::Bind(Graphics& gfx) noexcept -> void
{
    GetContext(gfx)->PSSetSamplers(0u, 1u, pSampler.GetAddressOf());
}
