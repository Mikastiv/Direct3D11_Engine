#pragma once

#include "Drawable.hpp"
#include "IndexBuffer.hpp"

template <typename T>
class Entity : public Drawable
{
protected:
    static auto IsStaticInitialized() noexcept -> bool
    {
        return !staticBinds.empty();
    };
    static auto AddStaticBind(std::unique_ptr<Bindable> bindable) noexcept(!IS_DEBUG) -> void
    {
        assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bindable) != typeid(IndexBuffer));
        staticBinds.push_back(std::move(bindable));
    }
    auto AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer) noexcept(!IS_DEBUG) -> void
    {
        assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
        pIndexBuffer = indexBuffer.get();
        pStaticIndexBuffer = indexBuffer.get();
        staticBinds.push_back(std::move(indexBuffer));
    }
    auto SetIndexFromStatic() noexcept(!IS_DEBUG) -> void
    {
        assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
        assert("Failed to find index buffer in static binds" && pStaticIndexBuffer != nullptr);
        pIndexBuffer = pStaticIndexBuffer;
    }

private:
    auto GetStaticBinds() const noexcept -> const std::vector<std::unique_ptr<Bindable>>& override
    {
        return staticBinds;
    }

private:
    inline static const IndexBuffer* pStaticIndexBuffer = nullptr;
    inline static std::vector<std::unique_ptr<Bindable>> staticBinds{};
};