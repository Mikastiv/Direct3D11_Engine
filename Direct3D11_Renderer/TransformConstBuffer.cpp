#include "TransformConstBuffer.hpp"

TransformConstBuffer::TransformConstBuffer(Graphics& gfx, const Drawable& parent)
    : parent(parent)
{
    if (!pVertexCBuffer)
    {
        pVertexCBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx);
    }
}

auto TransformConstBuffer::Bind(Graphics& gfx) noexcept -> void
{
    const auto model = parent.GetTransformXM();
    const Transforms t{ DirectX::XMMatrixTranspose(model),
                        DirectX::XMMatrixTranspose(model * gfx.GetCameraView() * gfx.GetProjection()) };
    pVertexCBuffer->Update(gfx, t);
    pVertexCBuffer->Bind(gfx);
}
