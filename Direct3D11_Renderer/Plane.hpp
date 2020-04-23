#pragma once

#include "IndexedTriangleList.hpp"

class Plane
{
public:
    template <typename T>
    static auto Make(float size = 1.0f, size_t divisions = 10u) -> IndexedTriangleList<T>
    {
        const float divisionSize = size / divisions;
        const size_t nVertexPerLine = divisions + 1;
        const DirectX::XMFLOAT3 topLeft{ -size / 2.0f, size / 2.0f, 0.0f };
        std::vector<DirectX::XMFLOAT3> vertices{};

        const auto xValue = [=](size_t x) { return topLeft.x + (divisionSize * x); };
        const auto yValue = [=](size_t y) { return topLeft.y + (divisionSize * y); };
        for (size_t y = 0; y < nVertexPerLine; y++)
        {
            for (size_t x = 0; x < nVertexPerLine; x++)
            {
                vertices.emplace_back(xValue(x), yValue(y), 0.0f);
            }
        }

        std::vector<uint16_t> indices{};
        const auto getIndex = [=](size_t x, size_t y) { return uint16_t(y * nVertexPerLine + x); };
        for (size_t y = 0; y < nVertexPerLine - 1; y++)
        {
            for (size_t x = 0; x < nVertexPerLine - 1; x++)
            {
                indices.push_back(getIndex(x, y));
                indices.push_back(getIndex(x, y + 1));
                indices.push_back(getIndex(x + 1, y));

                indices.push_back(getIndex(x + 1, y));
                indices.push_back(getIndex(x, y + 1));
                indices.push_back(getIndex(x + 1, y + 1));
            }
        }

        std::vector<T> output{};
        output.resize(vertices.size());
        for (size_t i = 0; i < vertices.size(); i++)
        {
            output[i].pos = vertices[i];
        }

        return { std::move(output), std::move(indices) };
    }
};