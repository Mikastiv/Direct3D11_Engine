#pragma once

#include "IndexedTriangleList.hpp"

class Cone
{
public:
    template <typename T>
    static auto Make(float radius = 0.5f, float height = 2.0f, size_t divisions = 10u) -> IndexedTriangleList<T>
    {
        const auto baseVector = DirectX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
        const size_t nVerticesBase = divisions + 1;
        const float angleStep = DirectX::XM_2PI / nVerticesBase;

        std::vector<DirectX::XMFLOAT3> vertices{};
        const uint16_t iTopVertex = (uint16_t)vertices.size();
        vertices.emplace_back(0.0f, height, 0.0f);

        const uint16_t iBottomVertex = (uint16_t)vertices.size();
        vertices.emplace_back(0.0f, 0.0f, 0.0f);

        for (size_t i = 0; i < nVerticesBase; i++)
        {
            vertices.emplace_back();
            DirectX::XMStoreFloat3(&vertices.back(),
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

        std::vector<T> output{};
        output.resize(vertices.size());

        for (size_t i = 0; i < vertices.size(); i++)
        {
            output[i].pos = vertices[i];
        }

        return { std::move(output), std::move(indices) };
    }
};