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
        auto get_type() const noexcept -> Type;
        auto get_pos() const noexcept -> std::pair<int, int>;
        auto get_pos_x() const noexcept -> int;
        auto get_pos_y() const noexcept -> int;
        auto is_left_pressed() const noexcept -> bool;
        auto is_right_pressed() const noexcept -> bool;
        auto is_middle_pressed() const noexcept -> bool;

    private:
        Type type{};
        bool left_is_pressed = false;
        bool right_is_pressed = false;
        bool middle_is_pressed = false;
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
    auto get_pos() const noexcept -> std::pair<int, int>;
    auto get_pos_x() const noexcept -> int;
    auto get_pos_y() const noexcept -> int;
    auto is_left_pressed() const noexcept -> bool;
    auto is_right_pressed() const noexcept -> bool;
    auto is_middle_pressed() const noexcept -> bool;
    auto is_in_window() const noexcept -> bool;
    auto read_event() noexcept -> std::optional<Event>;
    auto event_queue_is_empty() const noexcept -> bool;
    auto clear_event_queue() noexcept -> void;

private:
    auto on_left_pressed(int x, int y) noexcept -> void;
    auto on_left_released(int x, int y) noexcept -> void;
    auto on_right_pressed(int x, int y) noexcept -> void;
    auto on_right_released(int x, int y) noexcept -> void;
    auto on_middle_pressed(int x, int y) noexcept -> void;
    auto on_middle_released(int x, int y) noexcept -> void;
    auto on_mouse_move(int x, int y) noexcept -> void;
    auto on_mouse_enter() noexcept -> void;
    auto on_mouse_leave() noexcept -> void;
    auto on_wheel_up(int x, int y) noexcept -> void;
    auto on_wheel_down(int x, int y) noexcept -> void;
    auto on_wheel_delta(int x, int y, int delta) noexcept -> void;
    auto trim_buffer() noexcept -> void;

private:
    static constexpr unsigned int buffer_size = 16u;

private:
    bool left_is_pressed = false;
    bool right_is_pressed = false;
    bool middle_is_pressed = false;
    bool mouse_is_in_window = false;
    int x = 0;
    int y = 0;
    int wheel_delta_carry = 0;
    std::queue<Event> buffer{};
};