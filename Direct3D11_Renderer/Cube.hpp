#pragma once

#include "IndexedTriangleList.hpp"

class Cube
{
public:
    template <typename T>
    static auto Make(float size = 1u) -> IndexedTriangleList<T>
    {
        const float side = size / 2.0f;

        std::vector<DirectX::XMFLOAT3> vertices;
        vertices.emplace_back(-side, -side, -side);  // 0
        vertices.emplace_back(side, -side, -side);   // 1
        vertices.emplace_back(-side, side, -side);   // 2
        vertices.emplace_back(side, side, -side);    // 3
        vertices.emplace_back(-side, -side, side);   // 4
        vertices.emplace_back(side, -side, side);    // 5
        vertices.emplace_back(-side, side, side);    // 6
        vertices.emplace_back(side, side, side);     // 7

        std::vector<T> output(vertices.size());
        for (size_t i = 0; i < vertices.size(); i++)
        {
            output[i].pos = vertices[i];
        }
        return { std::move(output), { 0, 2, 1, 2, 3, 1, 1, 3, 5, 3, 7, 5, 2, 6, 3, 3, 6, 7,
                                      4, 5, 7, 4, 7, 6, 0, 4, 2, 2, 4, 6, 0, 1, 4, 1, 5, 4 } };
    }
};