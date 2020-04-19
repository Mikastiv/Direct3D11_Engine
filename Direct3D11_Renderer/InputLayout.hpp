#pragma once

#include "Bindable.hpp"

class InputLayout : public Bindable
{
public:
    InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode);
    auto Bind(Graphics& gfx) noexcept -> void override;

protected:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout{};
};