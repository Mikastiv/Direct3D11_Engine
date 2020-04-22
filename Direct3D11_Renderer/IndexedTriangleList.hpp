#pragma once

#include <DirectXMath.h>
#include <vector>

template <typename T>
struct IndexedTriangleList
{
public:
    IndexedTriangleList() = default;
    IndexedTriangleList(std::vector<T> vertices, std::vector<uint16_t> indices)
        : vertices(std::move(vertices))
        , indices(std::move(indices))
    {
        assert(this->vertices.size() > 2);
        assert(this->indices.size() % 3 == 0);
    }
    auto Transform(DirectX::FXMMATRIX matrix) -> void
    {
        for (auto& v : vertices)
        {
            const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.pos);
            DirectX::XMStoreFloat3(&v.pos, DirectX::XMVector3Transform(pos, matrix));
        }
    }

public:
    std::vector<T> vertices{};
    std::vector<uint16_t> indices{};
};