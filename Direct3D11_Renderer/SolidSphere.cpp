#include "SolidSphere.hpp"
#include "IcoSphere.hpp"
#include "BindableBase.hpp"

SolidSphere::SolidSphere(Graphics& gfx, float radius)
{
    if (!IsStaticInitialized())
    {
        struct Vertex
        {
            DirectX::XMFLOAT3 pos;
        };

        auto model = IcoSphere::Make<Vertex>();
        model.Transform(DirectX::XMMatrixScaling(radius, radius, radius));
        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

        auto pVS = std::make_unique<VertexShader>(gfx, L"Shaders\\ByteCode\\SolidVS.cso");
        const auto pVSByte = pVS->GetByteCode();
        AddStaticBind(std::move(pVS));

        AddStaticBind(std::make_unique<PixelShader>(gfx, L"Shaders\\ByteCode\\SolidPS.cso"));

        struct PSColorConstant
        {
            DirectX::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
            float padding;
        } colorConst;

        AddStaticBind(std::make_unique<PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> desc{
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexFromStatic();
    }

    AddBind(std::make_unique<TransformConstBuffer>(gfx, *this));
}

auto SolidSphere::Update(float deltaTime) noexcept -> void
{
}

auto SolidSphere::SetPos(DirectX::XMFLOAT3 pos) noexcept -> void
{
    this->pos = pos;
}

auto SolidSphere::GetTransformXM() const noexcept -> DirectX::XMMATRIX
{
    return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}
