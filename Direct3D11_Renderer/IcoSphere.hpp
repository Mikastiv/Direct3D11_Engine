#pragma once

#include "IndexedTriangleList.hpp"

#include <cmath>
#include <algorithm>
#include <unordered_map>

class IcoSphere
{
public:
    template <typename T>
    static auto Make(uint16_t tessellation = 2u) -> IndexedTriangleList<T>
    {
        std::vector<T> vertices{};
        const auto AddVertex = [&vertices](float x, float y, float z) {
            vertices.emplace_back();
            const float length = std::sqrt(x * x + y * y + z * z);
            vertices.back().pos = { x / length, y / length, z / length };
        };

        const float t = (1.0f + std::sqrt(5.0f)) / 2.0f;
        AddVertex(-1.0f, t, 0.0f);
        AddVertex(1.0f, t, 0.0f);
        AddVertex(-1.0f, -t, 0.0f);
        AddVertex(1.0f, -t, 0.0f);

        AddVertex(0.0f, -1.0f, t);
        AddVertex(0.0f, 1.0f, t);
        AddVertex(0.0f, -1.0f, -t);
        AddVertex(0.0f, 1.0f, -t);

        AddVertex(t, 0.0f, -1.0f);
        AddVertex(t, 0.0f, 1.0f);
        AddVertex(-t, 0.0f, -1.0f);
        AddVertex(-t, 0.0f, 1.0f);

        std::vector<uint16_t> indices{};

        // Create 20 triangles
        indices.push_back(0);
        indices.push_back(11);
        indices.push_back(5);
        indices.push_back(0);
        indices.push_back(5);
        indices.push_back(1);
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(7);
        indices.push_back(0);
        indices.push_back(7);
        indices.push_back(10);
        indices.push_back(0);
        indices.push_back(10);
        indices.push_back(11);

        indices.push_back(1);
        indices.push_back(5);
        indices.push_back(9);
        indices.push_back(5);
        indices.push_back(11);
        indices.push_back(4);
        indices.push_back(11);
        indices.push_back(10);
        indices.push_back(2);
        indices.push_back(10);
        indices.push_back(7);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(1);
        indices.push_back(8);

        indices.push_back(3);
        indices.push_back(9);
        indices.push_back(4);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(2);
        indices.push_back(6);
        indices.push_back(3);
        indices.push_back(6);
        indices.push_back(8);
        indices.push_back(3);
        indices.push_back(8);
        indices.push_back(9);

        indices.push_back(4);
        indices.push_back(9);
        indices.push_back(5);
        indices.push_back(2);
        indices.push_back(4);
        indices.push_back(11);
        indices.push_back(6);
        indices.push_back(2);
        indices.push_back(10);
        indices.push_back(8);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(9);
        indices.push_back(8);
        indices.push_back(1);

        std::unordered_map<uint16_t, uint16_t> middlePointCache{};
        const auto GetMiddlePoint = [&](uint16_t p0, uint16_t p1) {
            uint16_t smallerIndex = std::min(p0, p1);
            uint16_t greaterIndex = std::max(p0, p1);
            uint16_t key = (smallerIndex << 8) + greaterIndex;

            if (const auto it = middlePointCache.find(key); it != middlePointCache.end())
            {
                return it->second;
            }

            DirectX::XMFLOAT3& point0 = vertices[p0].pos;
            DirectX::XMFLOAT3& point1 = vertices[p1].pos;
            DirectX::XMFLOAT3 middle = { (point0.x + point1.x) / 2.0f,
                                         (point0.y + point1.y) / 2.0f,
                                         (point0.z + point1.z) / 2.0f };

            uint16_t index = (uint16_t)vertices.size();
            AddVertex(middle.x, middle.y, middle.z);
            middlePointCache.insert({ key, index });
            return index;
        };

        for (size_t i = 0; i < tessellation; i++)
        {
            std::vector<uint16_t> indices2{};
            for (uint16_t j = 0; j < (uint16_t)indices.size(); j += 3)
            {
                uint16_t a = GetMiddlePoint(indices[j], indices[j + 1]);
                uint16_t b = GetMiddlePoint(indices[j + 1], indices[j + 2]);
                uint16_t c = GetMiddlePoint(indices[j + 2], indices[j]);

                indices2.push_back(indices[j]);
                indices2.push_back(a);
                indices2.push_back(c);

                indices2.push_back(indices[j + 1]);
                indices2.push_back(b);
                indices2.push_back(a);

                indices2.push_back(indices[j + 2]);
                indices2.push_back(c);
                indices2.push_back(b);

                indices2.push_back(a);
                indices2.push_back(b);
                indices2.push_back(c);
            }
            indices = std::move(indices2);
        }

        return { std::move(vertices), std::move(indices) };
    }
};