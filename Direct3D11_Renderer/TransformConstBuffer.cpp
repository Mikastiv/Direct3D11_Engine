#include "TransformConstBuffer.hpp"

TransformConstBuffer::TransformConstBuffer(Graphics& gfx, const Drawable& parent, UINT slot)
    : parent(parent)
{
    if (!pVertexCBuffer)
    {
        pVertexCBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
    }
}

auto TransformConstBuffer::Bind(Graphics& gfx) noexcept -> void
{
    const auto modelView = parent.GetTransformXM() * gfx.GetCameraView();
    const Transforms t{ DirectX::XMMatrixTranspose(modelView),
                        DirectX::XMMatrixTranspose(modelView * gfx.GetProjection()) };
    pVertexCBuffer->Update(gfx, t);
    pVertexCBuffer->Bind(gfx);
}
