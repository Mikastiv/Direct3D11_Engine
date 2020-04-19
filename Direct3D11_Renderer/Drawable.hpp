#pragma once

#include "Graphics.hpp"
#include "DebugMacro.hpp"

#include <DirectXMath.h>
#include <memory>

class Bindable;
class IndexBuffer;

class Drawable
{
public:
    Drawable() = default;
    Drawable(const Drawable&) = delete;
    Drawable(Drawable&&) = delete;
    virtual ~Drawable() = default;
    auto operator=(const Drawable&) -> Drawable& = delete;
    auto operator=(Drawable &&) -> Drawable& = delete;
    virtual auto GetTransformXM() const noexcept -> DirectX::XMMATRIX = 0;
    virtual auto Update(float deltaTime) noexcept -> void = 0;
    auto Draw(Graphics& gfx) const noexcept(!IS_DEBUG) -> void;
    auto AddBind(std::unique_ptr<Bindable> bindable) noexcept(!IS_DEBUG) -> void;
    auto AddIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer) noexcept -> void;

private:
    const IndexBuffer* pIndexBuffer = nullptr;
    std::vector<std::unique_ptr<Bindable>> binds{};
};