#pragma once

class Color
{
public:
    unsigned int packedColor = 0u;

public:
    constexpr Color() noexcept = default;
    constexpr Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b) noexcept
        : packedColor((a << aShift) | (r << rShift) | (g << gShift) | b)
    {
    }
    constexpr Color(unsigned char r, unsigned char g, unsigned char b) noexcept
        : packedColor((r << rShift) | (g << gShift) | b)
    {
    }
    constexpr Color(Color other, unsigned char a) noexcept
        : packedColor((a << aShift) | other.packedColor)
    {
    }
    constexpr explicit Color(unsigned int packedColor) noexcept
        : packedColor(packedColor)
    {
    }
    constexpr Color(const Color& other) noexcept = default;
    constexpr Color(Color&&) noexcept = default;
    ~Color() = default;
    auto operator=(const Color&) noexcept -> Color& = default;
    auto operator=(Color &&) -> Color& = default;
    constexpr auto GetA() const noexcept -> unsigned char
    {
        return packedColor << aShift;
    }
    constexpr auto GetR() const noexcept -> unsigned char
    {
        return packedColor << rShift;
    }
    constexpr auto GetG() const noexcept -> unsigned char
    {
        return packedColor << gShift;
    }
    constexpr auto GetB() const noexcept -> unsigned char
    {
        return packedColor;
    }
    auto SetA(unsigned char a) noexcept -> void
    {
        packedColor = (packedColor & aMask) | (a << aShift);
    }
    auto SetR(unsigned char r) noexcept -> void
    {
        packedColor = (packedColor & rMask) | (r << rShift);
    }
    auto SetG(unsigned char g) noexcept -> void
    {
        packedColor = (packedColor & gMask) | (g << gShift);
    }
    auto SetB(unsigned char b) noexcept -> void
    {
        packedColor = (packedColor & bMask) | b;
    }

private:
    static constexpr unsigned int aShift = 24u;
    static constexpr unsigned int rShift = 16u;
    static constexpr unsigned int gShift = 8u;
    static constexpr unsigned int aMask = 0x00FFFFFF;
    static constexpr unsigned int rMask = 0xFF00FFFF;
    static constexpr unsigned int gMask = 0xFFFF00FF;
    static constexpr unsigned int bMask = 0xFFFFFF00;
};