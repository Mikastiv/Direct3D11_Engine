#pragma once

#include <queue>
#include <optional>

class Mouse
{
    friend class Window;

public:
    class Event
    {
    public:
        enum class Type
        {
            LPress,
            LRelease,
            RPress,
            RRelease,
            MPress,
            MRelease,
            WheelUp,
            WheelDown,
            Move,
            Enter,
            Leave
        };

    public:
        Event(Type type, const Mouse& parent) noexcept;
        Event(Type type, int x, int y, const Mouse& parent) noexcept;
        auto GetType() const noexcept -> Type;
        auto GetPos() const noexcept -> std::pair<int, int>;
        auto GetPosX() const noexcept -> int;
        auto GetPosY() const noexcept -> int;
        auto LeftIsPressed() const noexcept -> bool;
        auto RightIsPressed() const noexcept -> bool;
        auto MiddleIsPressed() const noexcept -> bool;

    private:
        Type type{};
        bool leftIsPressed = false;
        bool rightIsPressed = false;
        bool middleIsPressed = false;
        int x = 0;
        int y = 0;
    };

public:
    Mouse() = default;
    Mouse(const Mouse&) = delete;
    Mouse(Mouse&&) = delete;
    ~Mouse() = default;
    auto operator=(const Mouse&) -> Mouse& = delete;
    auto operator=(Mouse &&) -> Mouse& = delete;
    auto GetPos() const noexcept -> std::pair<int, int>;
    auto GetPosX() const noexcept -> int;
    auto GetPosY() const noexcept -> int;
    auto LeftIsPressed() const noexcept -> bool;
    auto RightIsPressed() const noexcept -> bool;
    auto MiddleIsPressed() const noexcept -> bool;
    auto IsInWindow() const noexcept -> bool;
    auto ReadEvent() noexcept -> std::optional<Event>;
    auto EventQueueIsEmpty() const noexcept -> bool;
    auto ClearEventQueue() noexcept -> void;

private:
    auto OnLeftPressed(int x, int y) noexcept -> void;
    auto OnLeftReleased(int x, int y) noexcept -> void;
    auto OnRightPressed(int x, int y) noexcept -> void;
    auto OnRightReleased(int x, int y) noexcept -> void;
    auto OnMiddlePressed(int x, int y) noexcept -> void;
    auto OnMiddleReleased(int x, int y) noexcept -> void;
    auto OnMouseMove(int x, int y) noexcept -> void;
    auto OnMouseEnter() noexcept -> void;
    auto OnMouseLeave() noexcept -> void;
    auto OnWheelUp(int x, int y) noexcept -> void;
    auto OnWheelDown(int x, int y) noexcept -> void;
    auto OnWheelDelta(int x, int y, int delta) noexcept -> void;
    auto TrimBuffer() noexcept -> void;

private:
    static constexpr unsigned int bufferSize = 16u;

private:
    bool leftIsPressed = false;
    bool rightIsPressed = false;
    bool middleIsPressed = false;
    bool mouseIsInWindow = false;
    int x = 0;
    int y = 0;
    int wheelDeltaCarry = 0;
    std::queue<Event> buffer{};
};