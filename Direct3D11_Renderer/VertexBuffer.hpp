#pragma once

#include "Bindable.hpp"
#include "GraphicsMacros.hpp"

class VertexBuffer : public Bindable
{
public:
    template <typename V>
    VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
        : stride(sizeof(V))
    {
        INFOMAN(gfx);

        D3D11_BUFFER_DESC desc{};
        desc.ByteWidth = UINT(sizeof(V) * vertices.size());
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = 0u;
        desc.MiscFlags = 0u;
        desc.StructureByteStride = sizeof(V);

        D3D11_SUBRESOURCE_DATA data{};
        data.pSysMem = vertices.data();

        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&desc, &data, &pVertexBuffer));
    }
    auto Bind(Graphics& gfx) noexcept -> void override;

protected:
    UINT stride{};
    UINT offset = 0u;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer{};
};