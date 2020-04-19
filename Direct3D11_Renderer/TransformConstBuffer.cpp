#include "TransformConstBuffer.hpp"

TransformConstBuffer::TransformConstBuffer(Graphics& gfx, const Drawable& parent)
    : vertexCBuffer(gfx)
    , parent(parent)
{
}

auto TransformConstBuffer::Bind(Graphics& gfx) noexcept -> void
{
    vertexCBuffer.Update(gfx, DirectX::XMMatrixTranspose(parent.GetTransformXM() * gfx.GetProjection()));
    vertexCBuffer.Bind(gfx);
}
