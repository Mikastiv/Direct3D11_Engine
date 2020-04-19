#include "Mouse.hpp"
#include "WinDefines.hpp"

Mouse::Event::Event(Type type, const Mouse& parent) noexcept
    : type(type)
    , leftIsPressed(parent.leftIsPressed)
    , rightIsPressed(parent.rightIsPressed)
    , middleIsPressed(parent.middleIsPressed)
    , x(parent.x)
    , y(parent.y)
{
}

Mouse::Event::Event(Type type, int x, int y, const Mouse& parent) noexcept
    : type(type)
    , leftIsPressed(parent.leftIsPressed)
    , rightIsPressed(parent.rightIsPressed)
    , middleIsPressed(parent.middleIsPressed)
    , x(x)
    , y(y)
{
}

auto Mouse::Event::GetType() const noexcept -> Type
{
    return type;
}

auto Mouse::Event::GetPos() const noexcept -> std::pair<int, int>
{
    return { x, y };
}

auto Mouse::Event::GetPosX() const noexcept -> int
{
    return x;
}

auto Mouse::Event::GetPosY() const noexcept -> int
{
    return y;
}

auto Mouse::Event::LeftIsPressed() const noexcept -> bool
{
    return leftIsPressed;
}

auto Mouse::Event::RightIsPressed() const noexcept -> bool
{
    return rightIsPressed;
}

auto Mouse::Event::MiddleIsPressed() const noexcept -> bool
{
    return middleIsPressed;
}

auto Mouse::OnLeftPressed(int x, int y) noexcept -> void
{
    leftIsPressed = true;
    buffer.emplace(Event::Type::LPress, x, y, *this);
    TrimBuffer();
}

auto Mouse::OnLeftReleased(int x, int y) noexcept -> void
{
    leftIsPressed = false;
    buffer.emplace(Event::Type::LRelease, x, y, *this);
    TrimBuffer();
}

auto Mouse::OnRightPressed(int x, int y) noexcept -> void
{
    rightIsPressed = true;
    buffer.emplace(Event::Type::RPress, x, y, *this);
    TrimBuffer();
}

auto Mouse::OnRightReleased(int x, int y) noexcept -> void
{
    rightIsPressed = false;
    buffer.emplace(Event::Type::RRelease, x, y, *this);
    TrimBuffer();
}

auto Mouse::OnMiddlePressed(int x, int y) noexcept -> void
{
    middleIsPressed = true;
    buffer.emplace(Event::Type::MPress, x, y, *this);
    TrimBuffer();
}

auto Mouse::OnMiddleReleased(int x, int y) noexcept -> void
{
    middleIsPressed = false;
    buffer.emplace(Event::Type::MRelease, x, y, *this);
    TrimBuffer();
}

auto Mouse::OnMouseMove(int x, int y) noexcept -> void
{
    this->x = x;
    this->y = y;
    buffer.emplace(Event::Type::Move, *this);
    TrimBuffer();
}

auto Mouse::OnMouseEnter() noexcept -> void
{
    mouseIsInWindow = true;
    buffer.emplace(Event::Type::Enter, *this);
    TrimBuffer();
}

auto Mouse::OnMouseLeave() noexcept -> void
{
    mouseIsInWindow = false;
    buffer.emplace(Event::Type::Leave, *this);
    TrimBuffer();
}

auto Mouse::OnWheelUp(int x, int y) noexcept -> void
{
    buffer.emplace(Event::Type::WheelUp, x, y, *this);
    TrimBuffer();
}

auto Mouse::OnWheelDown(int x, int y) noexcept -> void
{
    buffer.emplace(Event::Type::WheelDown, x, y, *this);
    TrimBuffer();
}

auto Mouse::OnWheelDelta(int x, int y, int delta) noexcept -> void
{
    wheelDeltaCarry += delta;
    // generate events for every 120
    while (wheelDeltaCarry >= WHEEL_DELTA)
    {
        wheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp(x, y);
    }
    while (wheelDeltaCarry <= -WHEEL_DELTA)
    {
        wheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown(x, y);
    }
}

auto Mouse::TrimBuffer() noexcept -> void
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}

auto Mouse::GetPos() const noexcept -> std::pair<int, int>
{
    return { x, y };
}

auto Mouse::GetPosX() const noexcept -> int
{
    return x;
}

auto Mouse::GetPosY() const noexcept -> int
{
    return y;
}

auto Mouse::LeftIsPressed() const noexcept -> bool
{
    return leftIsPressed;
}

auto Mouse::RightIsPressed() const noexcept -> bool
{
    return rightIsPressed;
}

auto Mouse::MiddleIsPressed() const noexcept -> bool
{
    return middleIsPressed;
}

auto Mouse::IsInWindow() const noexcept -> bool
{
    return mouseIsInWindow;
}

auto Mouse::ReadEvent() noexcept -> std::optional<Event>
{
    if (buffer.empty())
    {
        return {};
    }

    Event e{ buffer.front() };
    buffer.pop();
    return e;
}

auto Mouse::EventQueueIsEmpty() const noexcept -> bool
{
    return buffer.empty();
}

auto Mouse::ClearEventQueue() noexcept -> void
{
    buffer = std::queue<Event>{};
}
