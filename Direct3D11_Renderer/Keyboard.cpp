#include "Keyboard.hpp"
#include "Mouse.hpp"

Keyboard::Event::Event(Type type, unsigned char code) noexcept
    : type(type)
    , code(code)
{
}

auto Keyboard::Event::IsPress() const noexcept -> bool
{
    return type == Type::Press;
}

auto Keyboard::Event::IsRelease() const noexcept -> bool
{
    return type == Type::Release;
}

auto Keyboard::Event::GetCode() const noexcept -> unsigned char
{
    return code;
}

template <typename T>
auto Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept -> void
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}

auto Keyboard::OnKeyPress(unsigned char keycode) noexcept -> void
{
    keyStates[keycode] = true;
    keyBuffer.emplace(Event::Type::Press, keycode);
    TrimBuffer(keyBuffer);
}

auto Keyboard::OnKeyRelease(unsigned char keycode) noexcept -> void
{
    keyStates[keycode] = false;
    keyBuffer.emplace(Event::Type::Release, keycode);
    TrimBuffer(keyBuffer);
}

auto Keyboard::OnChar(char character) noexcept -> void
{
    charbuffer.push(character);
    TrimBuffer(charbuffer);
}

auto Keyboard::ClearKeystates() noexcept -> void
{
    keyStates.reset();
}

auto Keyboard::KeyIsPressed(unsigned char keycode) const noexcept -> bool
{
    return keyStates[keycode];
}

auto Keyboard::ReadKey() noexcept -> std::optional<Event>
{
    if (keyBuffer.empty())
    {
        return {};
    }

    Event e{ keyBuffer.front() };
    keyBuffer.pop();
    return e;
}

auto Keyboard::KeyQueueIsEmpty() const noexcept -> bool
{
    return keyBuffer.empty();
}

auto Keyboard::ClearKeyQueue() noexcept -> void
{
    keyBuffer = std::queue<Event>{};
}

auto Keyboard::ReadChar() noexcept -> std::optional<unsigned char>
{
    if (charbuffer.empty())
    {
        return {};
    }

    unsigned char character = charbuffer.front();
    charbuffer.pop();
    return character;
}

auto Keyboard::CharQueueIsEmpty() const noexcept -> bool
{
    return charbuffer.empty();
}

auto Keyboard::ClearCharQueue() noexcept -> void
{
    charbuffer = std::queue<unsigned char>{};
}

auto Keyboard::ClearQueues() noexcept -> void
{
    ClearKeyQueue();
    ClearCharQueue();
}

auto Keyboard::EnableAutorepeat() noexcept -> void
{
    autorepeatEnabled = true;
}

auto Keyboard::DisableAutorepeat() noexcept -> void
{
    autorepeatEnabled = false;
}

auto Keyboard::AutorepeatIsEnabled() const noexcept -> bool
{
    return false;
}
