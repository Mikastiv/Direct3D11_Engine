#include "Box.hpp"
#include "BindableBase.hpp"
#include "Cube.hpp"
#include "Cone.hpp"
#include "Plane.hpp"
#include "Prism.hpp"

Box::Box(Graphics& gfx, float xOffset)
    : xOffset(xOffset)
{
    struct Vertex
    {
        DirectX::XMFLOAT3 pos;
    };

    if (!IsStaticInitialized())
    {
        // Vertex Buffer bind
        const auto model = Prism::Make<Vertex>();
        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

        // Index Buffer bind
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

        // Vertex Shader bind
        auto pVertexShader = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
        auto pVSByteCode = pVertexShader->GetByteCode();
        AddStaticBind(std::move(pVertexShader));

        // Pixel Shader bind
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

        // Constant buffer for face colors
        struct ConstBufferColors
        {
            struct
            {
                float r;
                float g;
                float b;
                float a;
            } face_colors[6];
        };
        const ConstBufferColors cbufColors = { {
            { 1.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f },
            { 1.0f, 1.0f, 0.0f },
            { 0.0f, 1.0f, 1.0f },
        } };
        AddStaticBind(std::make_unique<PixelConstantBuffer<ConstBufferColors>>(gfx, cbufColors));

        // Input Layout bind
        const std::vector<D3D11_INPUT_ELEMENT_DESC> desc{
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u }
        };
        AddStaticBind(std::make_unique<InputLayout>(gfx, desc, pVSByteCode));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexFromStatic();
    }

    AddBind(std::make_unique<TransformConstBuffer>(gfx, *this));
}

auto Box::Update(float deltaTime) noexcept -> void
{
    pitch = dPitch * deltaTime;
    yaw += dYaw * deltaTime;
    roll += dRoll * deltaTime;
}

auto Box::GetTransformXM() const noexcept -> DirectX::XMMATRIX
{
    return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * DirectX::XMMatrixTranslation(xOffset, 0.0f, 5.0f);
}
