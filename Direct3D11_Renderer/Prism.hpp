#pragma once

#include "IndexedTriangleList.hpp"

#include <cassert>

class Prism
{
public:
    template <typename T>
    static auto Make(float radius = 0.5f, uint16_t divisions = 3u) -> IndexedTriangleList<T>
    {
        assert(divisions > 2u);
        const float height = 1.0f;
        const float halfHeight = height / 2.0f;
        const auto topBaseVector = DirectX::XMVectorSet(0.0f, halfHeight, radius, 0.0f);
        const auto bottomBaseVector = DirectX::XMVectorSet(0.0f, -halfHeight, radius, 0.0f);
        const float angleStep = DirectX::XM_2PI / divisions;

        std::vector<T> vertices{};
        for (size_t i = 0; i < divisions; i++)
        {
            vertices.emplace_back();
            DirectX::XMStoreFloat3(
                &vertices.back().pos,
                DirectX::XMVector3Transform(topBaseVector, DirectX::XMMatrixRotationY(angleStep * i)));
        }
        for (size_t i = 0; i < divisions; i++)
        {
            vertices.emplace_back();
            DirectX::XMStoreFloat3(
                &vertices.back().pos,
                DirectX::XMVector3Transform(bottomBaseVector, DirectX::XMMatrixRotationY(angleStep * i)));
        }

        const uint16_t iTopVertex = (uint16_t)vertices.size();
        vertices.emplace_back();
        vertices.back().pos = { 0.0f, halfHeight, 0.0f };

        const uint16_t iBottomVertex = (uint16_t)vertices.size();
        vertices.emplace_back();
        vertices.back().pos = { 0.0f, -halfHeight, 0.0f };

        // Top and bottom faces
        std::vector<uint16_t> indices{};
        for (uint16_t i = 0; i < divisions - 1; i++)
        {
            // Top triangle
            indices.push_back(iTopVertex);
            indices.push_back(i);
            indices.push_back(i + 1);

            // Bottom triangle
            indices.push_back(divisions + i);
            indices.push_back(iBottomVertex);
            indices.push_back(divisions + i + 1);

            // Side face
            indices.push_back(i);
            indices.push_back(divisions + i);
            indices.push_back(i + 1);
            indices.push_back(i + 1);
            indices.push_back(divisions + i);
            indices.push_back(divisions + i + 1);
        }
        // Last top triangle
        indices.push_back(iTopVertex);
        indices.push_back(divisions - 1);
        indices.push_back(0);

        // Last bottom triangle
        indices.push_back(divisions * 2 - 1);
        indices.push_back(iBottomVertex);
        indices.push_back(divisions);

        // Last side faces
        indices.push_back(0);
        indices.push_back(divisions - 1);
        indices.push_back(divisions);
        indices.push_back(divisions);
        indices.push_back(divisions - 1);
        indices.push_back(divisions * 2 - 1);

        return { std::move(vertices), std::move(indices) };
    }
};