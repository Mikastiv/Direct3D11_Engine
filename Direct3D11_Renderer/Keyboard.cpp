#include "Keyboard.hpp"

Keyboard::Event::Event() noexcept
    : type(Type::Invalid)
    , code(0u)
{
}

Keyboard::Event::Event(Type type, unsigned char code) noexcept
    : type(type)
    , code(code)
{
}

auto Keyboard::Event::is_press() const noexcept -> bool
{
    return type == Type::Press;
}

auto Keyboard::Event::is_release() const noexcept -> bool
{
    return type == Type::Release;
}

auto Keyboard::Event::is_valid() const noexcept -> bool
{
    return type != Type::Invalid;
}

unsigned char Keyboard::Event::get_code() const noexcept
{
    return code;
}

template <typename T>
auto Keyboard::trim_buffer(std::queue<T>& buffer) noexcept -> void
{
    while (buffer.size() > buffer_size)
    {
        buffer.pop();
    }
}

auto Keyboard::on_key_press(unsigned char keycode) noexcept -> void
{
    keystates[keycode] = true;
    keybuffer.push({ Event::Type::Press, keycode });
    trim_buffer(keybuffer);
}

auto Keyboard::on_key_release(unsigned char keycode) noexcept -> void
{
    keystates[keycode] = false;
    keybuffer.push({ Event::Type::Release, keycode });
    trim_buffer(keybuffer);
}

auto Keyboard::on_char(char character) noexcept -> void
{
    charbuffer.push(character);
    trim_buffer(charbuffer);
}

auto Keyboard::clear_keystates() noexcept -> void
{
    keystates.reset();
}

auto Keyboard::key_is_pressed(unsigned char keycode) const noexcept -> bool
{
    return keystates[keycode];
}

auto Keyboard::read_key() noexcept -> Event
{
    if (keybuffer.empty())
    {
        return Event{};
    }

    Event e{ keybuffer.front() };
    keybuffer.pop();
    return e;
}

auto Keyboard::key_queue_is_empty() const noexcept -> bool
{
    return keybuffer.empty();
}

auto Keyboard::clear_key_queue() noexcept -> void
{
    keybuffer = std::queue<Event>{};
}

auto Keyboard::read_char() noexcept -> unsigned char
{
    if (charbuffer.empty())
    {
        return '\0';
    }

    unsigned char character = charbuffer.front();
    charbuffer.pop();
    return character;
}

auto Keyboard::char_queue_is_empty() const noexcept -> bool
{
    return charbuffer.empty();
}

auto Keyboard::clear_char_queue() noexcept -> void
{
    charbuffer = std::queue<unsigned char>{};
}

auto Keyboard::clear_queues() noexcept -> void
{
    clear_key_queue();
    clear_char_queue();
}

auto Keyboard::enable_autorepeat() noexcept -> void
{
    autorepeat_enabled = true;
}

auto Keyboard::disable_autorepeat() noexcept -> void
{
    autorepeat_enabled = false;
}

auto Keyboard::autorepeat_is_enabled() const noexcept -> bool
{
    return false;
}
