#include "TransformConstBuffer.hpp"

TransformConstBuffer::TransformConstBuffer(Graphics& gfx, const Drawable& parent)
    : parent(parent)
{
    if (!pVertexCBuffer)
    {
        pVertexCBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
    }
}

auto TransformConstBuffer::Bind(Graphics& gfx) noexcept -> void
{
    pVertexCBuffer->Update(gfx, DirectX::XMMatrixTranspose(parent.GetTransformXM() * gfx.GetProjection()));
    pVertexCBuffer->Bind(gfx);
}
