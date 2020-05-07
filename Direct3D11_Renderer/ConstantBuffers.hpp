#pragma once

#include "Bindable.hpp"
#include "GraphicsMacros.hpp"

template <typename T>
class ConstantBuffer : public Bindable
{
public:
    ConstantBuffer(Graphics& gfx, UINT slot = 0u)
        : slot(slot)
    {
        INFOMAN(gfx);

        D3D11_BUFFER_DESC desc{};
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0u;
        desc.ByteWidth = sizeof(T);
        desc.StructureByteStride = 0u;

        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&desc, nullptr, &pConstantBuffer));
    }
    ConstantBuffer(Graphics& gfx, const T& bufferData, UINT slot = 0u)
        : slot(slot)
    {
        INFOMAN(gfx);

        D3D11_BUFFER_DESC desc{};
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0u;
        desc.ByteWidth = sizeof(bufferData);
        desc.StructureByteStride = 0u;

        D3D11_SUBRESOURCE_DATA data{};
        data.pSysMem = &bufferData;

        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&desc, &data, &pConstantBuffer));
    }
    auto Update(Graphics& gfx, const T& bufferData) -> void
    {
        INFOMAN(gfx);

        D3D11_MAPPED_SUBRESOURCE resource{};
        GFX_THROW_INFO(GetContext(gfx)->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &resource));
        memcpy(resource.pData, &bufferData, sizeof(bufferData));
        GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
    }

protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer{};
    UINT slot = 0;
};

template <typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
public:
    VertexConstantBuffer(Graphics& gfx, UINT slot = 0u)
        : ConstantBuffer<T>(gfx, slot)
    {
    }
    VertexConstantBuffer(Graphics& gfx, const T& bufferData, UINT slot = 0u)
        : ConstantBuffer<T>(gfx, bufferData, slot)
    {
    }
    auto Bind(Graphics& gfx) noexcept -> void override
    {
        Bindable::GetContext(gfx)->VSSetConstantBuffers(this->slot, 1u, this->pConstantBuffer.GetAddressOf());
    }
};

template <typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
public:
    explicit PixelConstantBuffer(Graphics& gfx, UINT slot = 0u)
        : ConstantBuffer<T>(gfx, slot)
    {
    }
    PixelConstantBuffer(Graphics& gfx, const T& bufferData, UINT slot = 0u)
        : ConstantBuffer<T>(gfx, bufferData, slot)
    {
    }
    auto Bind(Graphics& gfx) noexcept -> void override
    {
        Bindable::GetContext(gfx)->PSSetConstantBuffers(this->slot, 1u, this->pConstantBuffer.GetAddressOf());
    }
};