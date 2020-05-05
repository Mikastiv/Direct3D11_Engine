#pragma once

#include "MikastivException.hpp"
#include "Color.hpp"
#include "DebugMacro.hpp"

#include <memory>

class Surface
{
public:
    using Color = Color;

public:
    class Exception : public MikastivException
    {
    public:
        Exception(int line, const char* file, std::string note);
        auto what() const noexcept -> const char* override;
        auto GetType() const noexcept -> const char* override;
        auto GetNote() const noexcept -> const std::string&;

    private:
        std::string note{};
    };

public:
    static auto FromFile(const std::wstring& file) -> Surface;

public:
    Surface(unsigned int width, unsigned int height) noexcept;
    Surface(const Surface& other) noexcept(!IS_DEBUG);
    Surface(Surface&& other) noexcept;
    ~Surface() = default;
    auto operator=(const Surface& rhs) noexcept(!IS_DEBUG) -> Surface&;
    auto operator=(Surface&& rhs) noexcept -> Surface&;
    auto Clear(Color fill) noexcept -> void;
    auto PutPixel(unsigned int x, unsigned int y, Color c) noexcept(!IS_DEBUG) -> void;
    auto GetPixel(unsigned int x, unsigned int y) const noexcept(!IS_DEBUG) -> Color;
    auto GetWidth() const noexcept -> unsigned int;
    auto GetHeight() const noexcept -> unsigned int;
    auto GetBufferPtr() noexcept -> Color*;
    auto GetBufferPtr() const noexcept -> const Color*;
    auto GetBufferCPtr() const noexcept -> const Color*;
    auto SaveBMP(const std::wstring& file) const -> void;

private:
    Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pColor) noexcept;

private:
    std::unique_ptr<Color[]> pBuffer{};
    unsigned int height = 0u;
    unsigned int width = 0u;
};