#pragma once

#include <queue>
#include <bitset>
#include <optional>

class Keyboard
{
    friend class Window;

public:
    class Event
    {
    public:
        enum class Type
        {
            Press,
            Release
        };

    public:
        Event(Type type, unsigned char code) noexcept;
        auto is_press() const noexcept -> bool;
        auto is_release() const noexcept -> bool;
        auto get_code() const noexcept -> unsigned char;

    private:
        Type type{};
        unsigned char code = 0u;
    };

public:
    Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard(Keyboard&&) = delete;
    ~Keyboard() = default;
    auto operator=(const Keyboard&) -> Keyboard& = delete;
    auto operator=(Keyboard &&) -> Keyboard& = delete;

    auto key_is_pressed(unsigned char keycode) const noexcept -> bool;
    // Read a key event from the queue
    auto read_key() noexcept -> std::optional<Event>;
    auto key_queue_is_empty() const noexcept -> bool;
    auto clear_key_queue() noexcept -> void;

    // Read a char from the queue
    auto read_char() noexcept -> std::optional<unsigned char>;
    auto char_queue_is_empty() const noexcept -> bool;
    auto clear_char_queue() noexcept -> void;

    // Clear key queue and char queue
    auto clear_queues() noexcept -> void;

    auto enable_autorepeat() noexcept -> void;
    auto disable_autorepeat() noexcept -> void;
    auto autorepeat_is_enabled() const noexcept -> bool;

private:
    template <typename T>
    auto trim_buffer(std::queue<T>& buffer) noexcept -> void;
    auto on_key_press(unsigned char keycode) noexcept -> void;
    auto on_key_release(unsigned char keycode) noexcept -> void;
    auto on_char(char character) noexcept -> void;
    auto clear_keystates() noexcept -> void;

private:
    static constexpr unsigned int n_keys = 256u;
    static constexpr unsigned int buffer_size = 16u;

private:
    bool autorepeat_enabled = false;
    std::bitset<n_keys> keystates{};
    std::queue<Event> keybuffer{};
    std::queue<unsigned char> charbuffer{};
};
