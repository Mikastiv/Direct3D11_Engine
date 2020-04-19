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
        auto IsPress() const noexcept -> bool;
        auto IsRelease() const noexcept -> bool;
        auto GetCode() const noexcept -> unsigned char;

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

    auto KeyIsPressed(unsigned char keycode) const noexcept -> bool;
    // Read a key event from the queue
    auto ReadKey() noexcept -> std::optional<Event>;
    auto KeyQueueIsEmpty() const noexcept -> bool;
    auto ClearKeyQueue() noexcept -> void;

    // Read a char from the queue
    auto ReadChar() noexcept -> std::optional<unsigned char>;
    auto CharQueueIsEmpty() const noexcept -> bool;
    auto ClearCharQueue() noexcept -> void;

    // Clear key queue and char queue
    auto ClearQueues() noexcept -> void;

    auto EnableAutorepeat() noexcept -> void;
    auto DisableAutorepeat() noexcept -> void;
    auto AutorepeatIsEnabled() const noexcept -> bool;

private:
    template <typename T>
    auto TrimBuffer(std::queue<T>& buffer) noexcept -> void;
    auto OnKeyPress(unsigned char keycode) noexcept -> void;
    auto OnKeyRelease(unsigned char keycode) noexcept -> void;
    auto OnChar(char character) noexcept -> void;
    auto ClearKeystates() noexcept -> void;

private:
    static constexpr unsigned int nKeys = 256u;
    static constexpr unsigned int bufferSize = 16u;

private:
    bool autorepeatEnabled = false;
    std::bitset<nKeys> keyStates{};
    std::queue<Event> keyBuffer{};
    std::queue<unsigned char> charbuffer{};
};
