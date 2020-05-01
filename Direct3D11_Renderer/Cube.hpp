#pragma once

#include "IndexedTriangleList.hpp"

class Cube
{
public:
    template <typename T>
    static auto Make() -> IndexedTriangleList<T>
    {
        const float size = 1.0f;
        const float side = size / 2.0f;

        std::vector<T> vertices(8);
        vertices[0].pos = { -side, -side, -side };  // 0
        vertices[1].pos = { side, -side, -side };   // 1
        vertices[2].pos = { -side, side, -side };   // 2
        vertices[3].pos = { side, side, -side };    // 3
        vertices[4].pos = { -side, -side, side };   // 4
        vertices[5].pos = { side, -side, side };    // 5
        vertices[6].pos = { -side, side, side };    // 6
        vertices[7].pos = { side, side, side };     // 7

        return { std::move(vertices), { 0, 2, 1, 2, 3, 1, 1, 3, 5, 3, 7, 5, 2, 6, 3, 3, 6, 7,
                                        4, 5, 7, 4, 7, 6, 0, 4, 2, 2, 4, 6, 0, 1, 4, 1, 5, 4 } };
    }

    template <typename T>
    static auto MakeSkinned() -> IndexedTriangleList<T>
    {
        const float size = 1.0f;
        const float side = size / 2.0f;

        // clang-format off
        //         0 ----- 1
        //         |       |
        //         |       |
        // 2 ----- 3 ----- 4 ----- 5 ----- 6
        // |       |       |       |       |
        // |       |       |       |       |
        // 7 ----- 8 ----- 9 ----- 10 ---- 11
        //         |       |
        //         |       |
        //         12 ---- 13
        // clang-format on

        const auto GetUNorm = [](float u) { return u / 4.0f; };
        const auto GetVNorm = [](float v) { return v / 3.0f; };

        std::vector<T> vertices(14);
        vertices[0].pos = { -side, side, side };  // 0
        vertices[0].uv = { GetUNorm(1.0f), GetVNorm(0.0f) };
        vertices[1].pos = { side, side, side };  // 1
        vertices[1].uv = { GetUNorm(2.0f), GetVNorm(0.0f) };
        vertices[2].pos = { -side, side, side };  // 2
        vertices[2].uv = { GetUNorm(0.0f), GetVNorm(1.0f) };
        vertices[3].pos = { -side, side, -side };  // 3
        vertices[3].uv = { GetUNorm(1.0f), GetVNorm(1.0f) };
        vertices[4].pos = { side, side, -side };  // 4
        vertices[4].uv = { GetUNorm(2.0f), GetVNorm(1.0f) };
        vertices[5].pos = { side, side, side };  // 5
        vertices[5].uv = { GetUNorm(3.0f), GetVNorm(1.0f) };
        vertices[6].pos = { -side, side, side };  // 6
        vertices[6].uv = { GetUNorm(4.0f), GetVNorm(1.0f) };
        vertices[7].pos = { -side, -side, side };  // 7
        vertices[7].uv = { GetUNorm(0.0f), GetVNorm(2.0f) };
        vertices[8].pos = { -side, -side, -side };  // 8
        vertices[8].uv = { GetUNorm(1.0f), GetVNorm(2.0f) };
        vertices[9].pos = { side, -side, -side };  // 9
        vertices[9].uv = { GetUNorm(2.0f), GetVNorm(2.0f) };
        vertices[10].pos = { side, -side, side };  // 10
        vertices[10].uv = { GetUNorm(3.0f), GetVNorm(2.0f) };
        vertices[11].pos = { -side, -side, side };  // 11
        vertices[11].uv = { GetUNorm(4.0f), GetVNorm(2.0f) };
        vertices[12].pos = { -side, -side, side };  // 12
        vertices[12].uv = { GetUNorm(1.0f), GetVNorm(3.0f) };
        vertices[13].pos = { side, -side, side };  // 13
        vertices[13].uv = { GetUNorm(2.0f), GetVNorm(3.0f) };

        return { std::move(vertices), { 0, 1, 3, 1, 4,  3, 2, 3, 7,  3, 8,  7,  3, 4, 8,  4, 9,  8,
                                        4, 5, 9, 5, 10, 9, 5, 6, 10, 6, 11, 10, 8, 9, 12, 9, 13, 12 } };
    }
};