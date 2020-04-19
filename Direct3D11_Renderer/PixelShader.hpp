#pragma once

#include "Bindable.hpp"

class PixelShader : public Bindable
{
public:
    PixelShader(Graphics& gfx, const std::wstring& path);
    auto Bind(Graphics& gfx) noexcept -> void override;

protected:
    Microsoft::WRL::ComPtr<ID3DBlob> pByteCode{};
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader{};
};