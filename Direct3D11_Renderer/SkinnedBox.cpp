#include "SkinnedBox.hpp"
#include "BindableBase.hpp"
#include "Surface.hpp"
#include "Texture.hpp"
#include "Sampler.hpp"
#include "Cube.hpp"

SkinnedBox::SkinnedBox(Graphics& gfx)
{
    struct Vertex
    {
        DirectX::XMFLOAT3 pos;
        struct
        {
            float u;
            float v;
        } uv;
    };

    if (!IsStaticInitialized())
    {
        // Vertex Buffer bind
        auto model = Cube::MakeSkinned<Vertex>();

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

        AddStaticBind(std::make_unique<Texture>(gfx, Surface::FromFile(L"Images\\cube.png")));

        AddStaticBind(std::make_unique<Sampler>(gfx));

        // Index Buffer bind
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

        // Vertex Shader bind
        auto pVertexShader = std::make_unique<VertexShader>(gfx, L"TextureVS.cso");
        auto pVSByteCode = pVertexShader->GetByteCode();
        AddStaticBind(std::move(pVertexShader));

        // Pixel Shader bind
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"TexturePS.cso"));

        // Input Layout bind
        const std::vector<D3D11_INPUT_ELEMENT_DESC> desc{
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
            { "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u }
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


auto SkinnedBox::Update(float deltaTime) noexcept -> void
{
    pitch = dPitch * deltaTime;
    yaw += dYaw * deltaTime;
    roll += dRoll * deltaTime;
}

auto SkinnedBox::GetTransformXM() const noexcept -> DirectX::XMMATRIX
{
    return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f);
}