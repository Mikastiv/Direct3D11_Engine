#include "Mouse.hpp"

Mouse::Event::Event() noexcept
{
}

Mouse::Event::Event(Type type, int x, int y, const Mouse& parent) noexcept
    : type(type)
    , left_is_pressed(parent.left_is_pressed)
    , right_is_pressed(parent.right_is_pressed)
    , middle_is_pressed(parent.middle_is_pressed)
    , x(x)
    , y(y)
{
}

auto Mouse::Event::is_valid() const noexcept -> bool
{
    return type != Type::Invalid;
}

auto Mouse::Event::get_type() const noexcept -> Type
{
    return type;
}

auto Mouse::Event::get_pos() const noexcept -> std::pair<int, int>
{
    return { x, y };
}

auto Mouse::Event::get_pos_x() const noexcept -> int
{
    return x;
}

auto Mouse::Event::get_pos_y() const noexcept -> int
{
    return y;
}

auto Mouse::Event::is_left_pressed() const noexcept -> bool
{
    return left_is_pressed;
}

auto Mouse::Event::is_right_pressed() const noexcept -> bool
{
    return right_is_pressed;
}

auto Mouse::Event::is_middle_pressed() const noexcept -> bool
{
    return middle_is_pressed;
}

auto Mouse::on_left_pressed(int x, int y) noexcept -> void
{
    left_is_pressed = true;
    buffer.emplace(Event::Type::LPress, x, y, *this);
    trim_buffer();
}

auto Mouse::on_left_released(int x, int y) noexcept -> void
{
    left_is_pressed = false;
    buffer.emplace(Event::Type::LRelease, x, y, *this);
    trim_buffer();
}

auto Mouse::on_right_pressed(int x, int y) noexcept -> void
{
    right_is_pressed = true;
    buffer.emplace(Event::Type::RPress, x, y, *this);
    trim_buffer();
}

auto Mouse::on_right_released(int x, int y) noexcept -> void
{
    right_is_pressed = false;
    buffer.emplace(Event::Type::RRelease, x, y, *this);
    trim_buffer();
}

auto Mouse::on_middle_pressed(int x, int y) noexcept -> void
{
    middle_is_pressed = true;
    buffer.emplace(Event::Type::MPress, x, y, *this);
    trim_buffer();
}

auto Mouse::on_middle_released(int x, int y) noexcept -> void
{
    middle_is_pressed = false;
    buffer.emplace(Event::Type::MRelease, x, y, *this);
    trim_buffer();
}

auto Mouse::on_mouse_move(int x, int y) noexcept -> void
{
    this->x = x;
    this->y = y;
    buffer.emplace(Event::Type::Move, x, y, *this);
    trim_buffer();
}

auto Mouse::on_wheel_up(int x, int y) noexcept -> void
{
    buffer.emplace(Event::Type::WheelUp, x, y, *this);
    trim_buffer();
}

auto Mouse::on_wheel_down(int x, int y) noexcept -> void
{
    buffer.emplace(Event::Type::WheelDown, x, y, *this);
    trim_buffer();
}

auto Mouse::trim_buffer() noexcept -> void
{
    while (buffer.size() > buffer_size)
    {
        buffer.pop();
    }
}

auto Mouse::get_pos() const noexcept -> std::pair<int, int>
{
    return { x, y };
}

auto Mouse::get_pos_x() const noexcept -> int
{
    return x;
}

auto Mouse::get_pos_y() const noexcept -> int
{
    return y;
}

auto Mouse::is_left_pressed() const noexcept -> bool
{
    return left_is_pressed;
}

auto Mouse::is_right_pressed() const noexcept -> bool
{
    return right_is_pressed;
}

auto Mouse::is_middle_pressed() const noexcept -> bool
{
    return middle_is_pressed;
}

auto Mouse::read_event() noexcept -> Event
{
    if (buffer.empty())
    {
        return Event{};
    }

    Event e{buffer.front()};
    buffer.pop();
    return e;
}

auto Mouse::event_queue_is_empty() const noexcept -> bool
{
    return buffer.empty();
}

auto Mouse::clear_event_queue() noexcept -> void
{
    buffer = std::queue<Event>{};
}
