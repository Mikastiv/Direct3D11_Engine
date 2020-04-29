#include "Texture.hpp"
#include "Surface.hpp"
#include "GraphicsMacros.hpp"

Texture::Texture(Graphics& gfx, const Surface& s)
{
    INFOMAN(gfx);

    D3D11_TEXTURE2D_DESC textureDesc{};
    textureDesc.Width = s.GetWidth();
    textureDesc.Height = s.GetHeight();
    textureDesc.MipLevels = 1u;
    textureDesc.ArraySize = 1u;
    textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    textureDesc.SampleDesc.Count = 1u;
    textureDesc.SampleDesc.Quality = 0u;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0u;
    textureDesc.MiscFlags = 0u;

    D3D11_SUBRESOURCE_DATA data{};
    data.pSysMem = s.GetBufferCPtr();
    data.SysMemPitch = s.GetWidth() * sizeof(Surface::Color);

    Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture{};
    GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&textureDesc, &data, &pTexture));

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc{};
    viewDesc.Format = textureDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MipLevels = 1u;
    viewDesc.Texture2D.MostDetailedMip = 0u;

    GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &viewDesc, &pTextureView));
}

auto Texture::Bind(Graphics& gfx) noexcept -> void
{
    GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}
