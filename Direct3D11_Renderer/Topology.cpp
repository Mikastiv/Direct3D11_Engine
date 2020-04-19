#include "Topology.hpp"

Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
    : type(type)
{
}

auto Topology::Bind(Graphics& gfx) noexcept -> void
{
    GetContext(gfx)->IASetPrimitiveTopology(type);
}
