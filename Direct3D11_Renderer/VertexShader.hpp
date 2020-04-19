#pragma once

#include "Bindable.hpp"

class VertexShader : public Bindable
{
public:
    VertexShader(Graphics& gfx, const std::wstring& path);
    auto Bind(Graphics& gfx) noexcept -> void override;
    auto GetByteCode() const noexcept -> ID3DBlob*;

protected:
    Microsoft::WRL::ComPtr<ID3DBlob> pByteCode{};
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader{};
};