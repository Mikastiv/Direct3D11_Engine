#pragma once

#include "Bindable.hpp"

class Texture : public Bindable
{
public:
    Texture(Graphics& gfx, const class Surface& s);
    auto Bind(Graphics& gfx) noexcept -> void override;

protected:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView{};
};