#pragma once

#include "IndexedTriangleList.hpp"

class Cone
{
public:
    template <typename T>
    static auto Make(float radius = 0.5f, float height = 1.5f, uint16_t divisions = 10u) -> IndexedTriangleList<T>
    {
        const float halfHeight = height / 2.0f;
        const auto baseVector = DirectX::XMVectorSet(0.0f, -halfHeight, radius, 0.0f);
        const float angleStep = DirectX::XM_2PI / divisions;

        std::vector<T> vertices{};
        const uint16_t iTopVertex = (uint16_t)vertices.size();
        vertices.emplace_back();
        vertices.back().pos = { 0.0f, halfHeight, 0.0f };

        const uint16_t iBottomVertex = (uint16_t)vertices.size();
        vertices.emplace_back();
        vertices.back().pos = { 0.0f, -halfHeight, 0.0f };

        for (size_t i = 0; i < divisions; i++)
        {
            vertices.emplace_back();
            DirectX::XMStoreFloat3(&vertices.back().pos,
                                   DirectX::XMVector3Transform(baseVector, DirectX::XMMatrixRotationY(angleStep * i)));
        }

        std::vector<uint16_t> indices{};
        const uint16_t iFirstBaseVertex = 2;
        for (uint16_t i = iFirstBaseVertex; i < uint16_t(vertices.size() - 1); i++)
        {
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(iTopVertex);

            indices.push_back(i);
            indices.push_back(iBottomVertex);
            indices.push_back(i + 1);
        }
        indices.push_back(uint16_t(vertices.size() - 1));
        indices.push_back(iBottomVertex);
        indices.push_back(iFirstBaseVertex);

        indices.push_back(uint16_t(vertices.size() - 1));
        indices.push_back(iFirstBaseVertex);
        indices.push_back(iTopVertex);

        return { std::move(vertices), std::move(indices) };
    }
};