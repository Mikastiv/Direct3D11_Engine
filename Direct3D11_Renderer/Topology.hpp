#pragma once

#include "Bindable.hpp"

class Topology : public Bindable
{
public:
    Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
    auto Bind(Graphics& gfx) noexcept -> void override;

protected:
    D3D11_PRIMITIVE_TOPOLOGY type{};
};