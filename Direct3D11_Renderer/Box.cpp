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
    std::uniform_real_distribution<float>& dRollDist,
    DirectX::XMFLOAT3 materialColor)
    : TestObject(
          rng, radiusDist, yRotOffsetDist, rotOffsetDist, dYRotationDist, dPitchDist, dYawDist, dRollDist, materialColor)
{
    struct Vertex
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT3 n;
    };

    if (!IsStaticInitialized())
    {
        // Vertex Buffer bind
        auto model = Cube::MakeIndependent<Vertex>();
        model.SetFlatNormals();
        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

        // Index Buffer bind
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

        // Vertex Shader bind
        auto pVertexShader = std::make_unique<VertexShader>(gfx, L"Shaders\\ByteCode\\PhongVS.cso");
        auto pVSByteCode = pVertexShader->GetByteCode();
        AddStaticBind(std::move(pVertexShader));

        // Pixel Shader bind
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"Shaders\\ByteCode\\PhongPS.cso"));

        // Input Layout bind
        const std::vector<D3D11_INPUT_ELEMENT_DESC> desc{
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
            { "Normal",
              0,
              DXGI_FORMAT_R32G32B32_FLOAT,
              0u,
              D3D11_APPEND_ALIGNED_ELEMENT,
              D3D11_INPUT_PER_VERTEX_DATA,
              0u }
        };

        AddStaticBind(std::make_unique<InputLayout>(gfx, desc, pVSByteCode));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexFromStatic();
    }

    AddBind(std::make_unique<TransformConstBuffer>(gfx, *this));

    struct PSMaterialConstant
    {
        DirectX::XMFLOAT3A color;
    } colorConst;
    colorConst.color = { materialColor.x, materialColor.y, materialColor.z };
    AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));
}
