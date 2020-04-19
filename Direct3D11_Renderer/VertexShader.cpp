#include "VertexShader.hpp"
#include "GraphicsMacros.hpp"

#include <d3dcompiler.h>

VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
{
    INFOMAN(gfx);

    GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pByteCode));
    GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
        pByteCode->GetBufferPointer(), pByteCode->GetBufferSize(), nullptr, &pVertexShader));
}

auto VertexShader::Bind(Graphics& gfx) noexcept -> void
{
    GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

auto VertexShader::GetByteCode() const noexcept -> ID3DBlob*
{
    return pByteCode.Get();
}
