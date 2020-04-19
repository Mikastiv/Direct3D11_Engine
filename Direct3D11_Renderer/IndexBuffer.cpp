#include "IndexBuffer.hpp"
#include "GraphicsMacros.hpp"

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<uint16_t>& indices)
    : count((UINT)indices.size())
{
    INFOMAN(gfx);

    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = UINT(sizeof(indices) * count);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags = 0u;
    desc.MiscFlags = 0u;
    desc.StructureByteStride = sizeof(uint16_t);

    D3D11_SUBRESOURCE_DATA data{};
    data.pSysMem = indices.data();

    GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&desc, &data, &pIndexBuffer));
}

auto IndexBuffer::Bind(Graphics& gfx) noexcept -> void
{
    GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

auto IndexBuffer::GetCount() const noexcept -> UINT
{
    return count;
}
