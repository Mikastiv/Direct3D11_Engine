#pragma once

class Color
{
public:
    unsigned int packedColor = 0u;

public:
    constexpr Color() noexcept
        : packedColor(0u)
    {
    }
    constexpr Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b) noexcept
        : packedColor((a << 24u) | (r << 16u) | (g << 8u) | b)
    {
    }
    constexpr Color(unsigned char r, unsigned char g, unsigned char b) noexcept
        : packedColor((r << 16u) | (g << 8u) | b)
    {
    }
    constexpr Color(Color other, unsigned char a) noexcept
        : packedColor((a << 24u) | other.packedColor)
    {
    }
    constexpr explicit Color(unsigned int packedColor) noexcept
        : packedColor(packedColor)
    {
    }
    constexpr Color(const Color& other) noexcept = default;
    constexpr Color(Color&&) noexcept = default;
    auto operator=(const Color&) noexcept -> Color& = default;
    auto operator=(Color &&) -> Color& = default;
    constexpr auto GetA() const noexcept -> unsigned char
    {
        return packedColor << 24u;
    }
    constexpr auto GetR() const noexcept -> unsigned char
    {
        return (packedColor << 16u) & 0xFFu;
    }
    constexpr auto GetG() const noexcept -> unsigned char
    {
        return (packedColor << 8u) & 0xFFu;
    }
    constexpr auto GetB() const noexcept -> unsigned char
    {
        return packedColor & 0xFFu;
    }
    auto SetA(unsigned char a) noexcept -> void
    {
        packedColor = (packedColor & 0x00FFFFFF) | (a << 24u);
    }
    auto SetR(unsigned char r) noexcept -> void
    {
        packedColor = (packedColor & 0xFF00FFFF) | (r << 16u);
    }
    auto SetG(unsigned char g) noexcept -> void
    {
        packedColor = (packedColor & 0xFFFF00FF) | (g << 8u);
    }
    auto SetB(unsigned char b) noexcept -> void
    {
        packedColor = (packedColor & 0xFFFFFF00) | b;
    }
};