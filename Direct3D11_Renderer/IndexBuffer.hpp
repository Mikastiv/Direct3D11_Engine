#pragma once

#include "Bindable.hpp"

class IndexBuffer : public Bindable
{
public:
    IndexBuffer(Graphics& gfx, const std::vector<uint16_t>& indices);
    auto Bind(Graphics& gfx) noexcept -> void override;
    auto GetCount() const noexcept -> UINT;

protected:
    UINT count{};
    Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer{};
};