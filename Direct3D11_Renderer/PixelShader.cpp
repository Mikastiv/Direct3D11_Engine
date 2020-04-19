#include "PixelShader.hpp"
#include "GraphicsMacros.hpp"

#include <d3dcompiler.h>

PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
{
    INFOMAN(gfx);

    GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pByteCode));
    GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(
        pByteCode->GetBufferPointer(), pByteCode->GetBufferSize(), nullptr, &pPixelShader));
}

auto PixelShader::Bind(Graphics& gfx) noexcept -> void
{
    GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
