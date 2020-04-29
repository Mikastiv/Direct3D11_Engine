#pragma once

#include "Bindable.hpp"

class Sampler : public Bindable
{
public:
    explicit Sampler(Graphics& gfx);
    auto Bind(Graphics& gfx) noexcept -> void override;

protected:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler{};
};