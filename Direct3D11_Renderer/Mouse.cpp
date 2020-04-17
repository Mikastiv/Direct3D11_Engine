#include "Mouse.hpp"
#include "WinDefines.hpp"

Mouse::Event::Event(Type type, const Mouse& parent) noexcept
    : type(type)
    , left_is_pressed(parent.left_is_pressed)
    , right_is_pressed(parent.right_is_pressed)
    , middle_is_pressed(parent.middle_is_pressed)
    , x(parent.x)
    , y(parent.y)
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
    buffer.emplace(Event::Type::Move, *this);
    trim_buffer();
}

auto Mouse::on_mouse_enter() noexcept -> void
{
    mouse_is_in_window = true;
    buffer.emplace(Event::Type::Enter, *this);
    trim_buffer();
}

auto Mouse::on_mouse_leave() noexcept -> void
{
    mouse_is_in_window = false;
    buffer.emplace(Event::Type::Leave, *this);
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

auto Mouse::on_wheel_delta(int x, int y, int delta) noexcept -> void
{
    wheel_delta_carry += delta;
    // generate events for every 120
    while (wheel_delta_carry >= WHEEL_DELTA)
    {
        wheel_delta_carry -= WHEEL_DELTA;
        on_wheel_up(x, y);
    }
    while (wheel_delta_carry <= -WHEEL_DELTA)
    {
        wheel_delta_carry += WHEEL_DELTA;
        on_wheel_down(x, y);
    }
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

auto Mouse::is_in_window() const noexcept -> bool
{
    return mouse_is_in_window;
}

auto Mouse::read_event() noexcept -> std::optional<Event>
{
    if (buffer.empty())
    {
        return {};
    }

    Event e{ buffer.front() };
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
