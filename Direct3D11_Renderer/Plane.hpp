#pragma once

#include "IndexedTriangleList.hpp"

#include <cassert>

class Plane
{
public:
    template <typename T>
    static auto Make(size_t divisionX = 1u, size_t divisionY = 1u) -> IndexedTriangleList<T>
    {
        assert(divisionX > 0);
        assert(divisionY > 0);

        const float size = 1.0f;
        const float divisionXSize = size / divisionX;
        const float divisionYSize = size / divisionY;
        const size_t nVerticesX = divisionX + 1;
        const size_t nVerticesY = divisionY + 1;
        const auto bottomLeft = DirectX::XMVectorSet(-size / 2.0f, size / 2.0f, 0.0f, 0.0f);
        std::vector<T> vertices(nVerticesX * nVerticesY);

        for (size_t y = 0, i = 0; y < nVerticesY; y++)
        {
            // Negative y to get topleft first (have same layout as uv coords)
            const auto yPos = (-float(y) * divisionYSize);
            for (size_t x = 0; x < nVerticesX; x++, i++)
            {
                const auto v =
                    DirectX::XMVectorAdd(bottomLeft, DirectX::XMVectorSet((float)x * divisionXSize, yPos, 0.0f, 0.0f));
                DirectX::XMStoreFloat3(&vertices[i].pos, v);
            }
        }

        std::vector<uint16_t> indices{};
        const auto GetIndex = [=](size_t x, size_t y) { return uint16_t(y * nVerticesX + x); };
        for (size_t y = 0; y < nVerticesY - 1; y++)
        {
            for (size_t x = 0; x < nVerticesX - 1; x++)
            {
                indices.push_back(GetIndex(x, y));
                indices.push_back(GetIndex(x + 1, y));
                indices.push_back(GetIndex(x, y + 1));

                indices.push_back(GetIndex(x + 1, y));
                indices.push_back(GetIndex(x + 1, y + 1));
                indices.push_back(GetIndex(x, y + 1));
            }
        }

        return { std::move(vertices), std::move(indices) };
    }
};