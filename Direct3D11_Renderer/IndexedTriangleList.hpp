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
    auto SetFlatNormals() -> void
    {
        assert(indices.size() % 3 == 0 && indices.size() > 0);

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            auto& v0 = vertices[indices[i]];
            auto& v1 = vertices[indices[i + 1]];
            auto& v2 = vertices[indices[i + 2]];

            const auto p0 = DirectX::XMLoadFloat3(&v0.pos);
            const auto p1 = DirectX::XMLoadFloat3(&v1.pos);
            const auto p2 = DirectX::XMLoadFloat3(&v2.pos);

            const auto p1p0 = DirectX::XMVectorSubtract(p1, p0);
            const auto p2p0 = DirectX::XMVectorSubtract(p2, p0);
            const auto n = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(p1p0, p2p0));

            DirectX::XMStoreFloat3(&v0.n, n);
            DirectX::XMStoreFloat3(&v1.n, n);
            DirectX::XMStoreFloat3(&v2.n, n);
        }
    }

public:
    std::vector<T> vertices{};
    std::vector<uint16_t> indices{};
};