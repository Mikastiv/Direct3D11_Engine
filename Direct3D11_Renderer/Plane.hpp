#pragma once

#include "IndexedTriangleList.hpp"

class Plane
{
public:
    template <typename T>
    static auto Make(size_t tessellation = 1u) -> IndexedTriangleList<T>
    {
        const float size = 1.0f;
        const float divisionSize = size / tessellation;
        const size_t nVertexPerLine = tessellation + 1;
        const DirectX::XMFLOAT3 bottomLeft{ -size / 2.0f, -size / 2.0f, 0.0f };
        std::vector<T> vertices{};

        const auto xValue = [=](size_t x) { return bottomLeft.x + (divisionSize * x); };
        const auto yValue = [=](size_t y) { return bottomLeft.y + (divisionSize * y); };
        for (size_t y = 0; y < nVertexPerLine; y++)
        {
            for (size_t x = 0; x < nVertexPerLine; x++)
            {
                vertices.emplace_back();
                // Negative y to get topleft first (have same layout as uv coords)
                vertices.back().pos = { xValue(x), -yValue(y), 0.0f };
            }
        }

        std::vector<uint16_t> indices{};
        const auto getIndex = [=](size_t x, size_t y) { return uint16_t(y * nVertexPerLine + x); };
        for (size_t y = 0; y < nVertexPerLine - 1; y++)
        {
            for (size_t x = 0; x < nVertexPerLine - 1; x++)
            {
                indices.push_back(getIndex(x, y));
                indices.push_back(getIndex(x + 1, y));
                indices.push_back(getIndex(x, y + 1));

                indices.push_back(getIndex(x + 1, y));
                indices.push_back(getIndex(x + 1, y + 1));
                indices.push_back(getIndex(x, y + 1));
            }
        }

        return { std::move(vertices), std::move(indices) };
    }
};