#include "Box.hpp"
#include "BindableBase.hpp"

Box::Box(Graphics& gfx)
{
    struct Vertex
    {
        struct
        {
            float x;
            float y;
            float z;
        } pos;
    };

    // Vertex Buffer bind
    const std::vector<Vertex> vertices{ { -1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f }, { -1.0f, 1.0f, -1.0f },
                                        { 1.0f, 1.0f, -1.0f },   { -1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f },
                                        { -1.0f, 1.0f, 1.0f },   { 1.0f, 1.0f, 1.0f } };
    AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

    // Index Buffer bind
    const std::vector<uint16_t> indices{ 0, 2, 1, 2, 3, 1, 1, 3, 5, 3, 7, 5, 2, 6, 3, 3, 6, 7,
                                         4, 5, 7, 4, 7, 6, 0, 4, 2, 2, 4, 6, 0, 1, 4, 1, 5, 4 };
    AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

    // Vertex Shader bind
    auto pVertexShader = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
    auto pVSByteCode = pVertexShader->GetByteCode();
    AddBind(std::move(pVertexShader));

    // Pixel Shader bind
    AddBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

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
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 1.0f },
    } };
    AddBind(std::make_unique<PixelConstantBuffer<ConstBufferColors>>(gfx, cbufColors));

    // Input Layout bind
    const std::vector<D3D11_INPUT_ELEMENT_DESC> desc{
        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u }
    };
    AddBind(std::make_unique<InputLayout>(gfx, desc, pVSByteCode));

    AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

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
    return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f);
}
