#include "InputLayout.hpp"
#include "GraphicsMacros.hpp"

InputLayout::InputLayout(Graphics& gfx,
                         const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
                         ID3DBlob* pVertexShaderByteCode)
{
    INFOMAN(gfx);

    GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(layout.data(),
                                                     (UINT)layout.size(),
                                                     pVertexShaderByteCode->GetBufferPointer(),
                                                     pVertexShaderByteCode->GetBufferSize(),
                                                     &pInputLayout));
}

auto InputLayout::Bind(Graphics& gfx) noexcept -> void
{
    GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}
