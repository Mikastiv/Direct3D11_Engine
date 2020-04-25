#pragma once

#include "IndexedTriangleList.hpp"

class Sphere
{
public:
    template <typename T>
    static auto Make(float radius = 1.0f, uint16_t latDivisions = 10u, uint16_t lonDivisions = 20u)
        -> IndexedTriangleList<T>
    {
        const auto baseVector = DirectX::XMVectorSet(0.0f, radius, 0.0f, 0.0f);
        const float latAngleStep = DirectX::XM_PI / latDivisions;
        const float lonAngleStep = DirectX::XM_2PI / lonDivisions;

        std::vector<T> vertices{};
        for (size_t i = 1; i < latDivisions; i++)
        {
            const auto baseLatVector =
                DirectX::XMVector3Transform(baseVector, DirectX::XMMatrixRotationX(latAngleStep * i));

            for (size_t j = 0; j < lonDivisions; j++)
            {
                vertices.emplace_back();
                DirectX::XMStoreFloat3(
                    &vertices.back().pos,
                    DirectX::XMVector3Transform(baseLatVector, DirectX::XMMatrixRotationY(lonAngleStep * j)));
            }
        }

        const auto getIndex = [=](uint16_t x, uint16_t y) { return y * lonDivisions + x; };

        std::vector<uint16_t> indices{};
        for (uint16_t y = 0; y < latDivisions - 2; y++)
        {
            for (uint16_t x = 0; x < lonDivisions - 1; x++)
            {
                indices.push_back(getIndex(x, y));
                indices.push_back(getIndex(x, y + 1));
                indices.push_back(getIndex(x + 1, y));
                indices.push_back(getIndex(x + 1, y));
                indices.push_back(getIndex(x, y + 1));
                indices.push_back(getIndex(x + 1, y + 1));
            }
            // Last square
            indices.push_back(getIndex(0, y));
            indices.push_back(getIndex(lonDivisions - 1, y));
            indices.push_back(getIndex(0, y + 1));
            indices.push_back(getIndex(0, y + 1));
            indices.push_back(getIndex(lonDivisions - 1, y));
            indices.push_back(getIndex(lonDivisions - 1, y + 1));
        }

        const uint16_t iTopVertex = (uint16_t)vertices.size();
        vertices.emplace_back();
        vertices.back().pos = { 0.0f, radius, 0.0f };

        const uint16_t iBottomVertex = (uint16_t)vertices.size();
        vertices.emplace_back();
        vertices.back().pos = { 0.0f, -radius, 0.0f };

        for (uint16_t i = 0; i < lonDivisions - 1; i++)
        {
            // Top cap
            indices.push_back(iTopVertex);
            indices.push_back(i);
            indices.push_back(i + 1);
            // Bottom cap
            indices.push_back(iBottomVertex);
            indices.push_back(lonDivisions * (latDivisions - 2) + i + 1);
            indices.push_back(lonDivisions * (latDivisions - 2) + i);
        }
        // Last top cap triangle
        indices.push_back(iTopVertex);
        indices.push_back(lonDivisions - 1);
        indices.push_back(0);
        // Last bottom cap triangle
        indices.push_back(iBottomVertex);
        indices.push_back(lonDivisions * (latDivisions - 2));
        indices.push_back(lonDivisions * (latDivisions - 2) + lonDivisions - 1);

        return { std::move(vertices), std::move(indices) };
    }
};