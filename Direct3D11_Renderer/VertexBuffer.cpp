#include "VertexBuffer.hpp"

auto VertexBuffer::Bind(Graphics& gfx) noexcept -> void
{
    GetContext(gfx)->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);
}
