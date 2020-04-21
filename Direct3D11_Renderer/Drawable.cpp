#include "Drawable.hpp"
#include "Bindable.hpp"
#include "IndexBuffer.hpp"

#include <cassert>
#include <typeinfo>

auto Drawable::Draw(Graphics& gfx) const noexcept(!IS_DEBUG) -> void
{
    for (auto& b : binds)
    {
        b->Bind(gfx);
    }
    for (auto& b : GetStaticBinds())
    {
        b->Bind(gfx);
    }
    gfx.DrawIndexed(pIndexBuffer->GetCount());
}

auto Drawable::AddBind(std::unique_ptr<Bindable> bindable) noexcept(!IS_DEBUG) -> void
{
    assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bindable) != typeid(IndexBuffer));
    binds.push_back(std::move(bindable));
}

auto Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer) noexcept(!IS_DEBUG) -> void
{
    assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
    pIndexBuffer = indexBuffer.get();
    binds.push_back(std::move(indexBuffer));
}
