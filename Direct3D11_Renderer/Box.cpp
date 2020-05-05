#include "Box.hpp"
#include "BindableBase.hpp"
#include "Cube.hpp"

Box::Box(
    Graphics& gfx,
    std::mt19937& rng,
    std::uniform_real_distribution<float>& radiusDist,
    std::uniform_real_distribution<float>& yRotOffsetDist,
    std::uniform_real_distribution<float>& rotOffsetDist,
    std::uniform_real_distribution<float>& dYRotationDist,
    std::uniform_real_distribution<float>& dPitchDist,
    std::uniform_real_distribution<float>& dYawDist,
    std::uniform_real_distribution<float>& dRollDist)
    : TestObject(rng, radiusDist, yRotOffsetDist, rotOffsetDist, dYRotationDist, dPitchDist, dYawDist, dRollDist)
{
    struct Vertex
    {
        DirectX::XMFLOAT3 pos;
    };

    if (!IsStaticInitialized())
    {
        // Vertex Buffer bind
        const auto model = Cube::Make<Vertex>();
        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

        // Index Buffer bind
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

        // Vertex Shader bind
        auto pVertexShader = std::make_unique<VertexShader>(gfx, L"Shaders\\ByteCode\\ColorIndexVS.cso");
        auto pVSByteCode = pVertexShader->GetByteCode();
        AddStaticBind(std::move(pVertexShader));

        // Pixel Shader bind
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"Shaders\\ByteCode\\ColorIndexPS.cso"));

        // Constant buffer for face colors
        struct ConstBufferColors
        {
            struct
            {
                float r;
                float g;
                float b;
                float a;
            } face_colors[8];
        };
        const ConstBufferColors cbufColors = { { { 1.0f, 0.0f, 0.0f },
                                                 { 1.0f, 1.0f, 0.0f },
                                                 { 1.0f, 0.0f, 1.0f },
                                                 { 0.0f, 1.0f, 0.0f },
                                                 { 0.0f, 1.0f, 1.0f },
                                                 { 0.0f, 0.0f, 1.0f },
                                                 { 0.5f, 0.5f, 0.5f },
                                                 { 0.5f, 0.0f, 0.5f } } };

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
