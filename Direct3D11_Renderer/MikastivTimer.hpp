#pragma once

#include <chrono>

class Timer
{
private:
    std::chrono::steady_clock::time_point last;

public:
    Timer()
        : last{ std::chrono::steady_clock::now() }
    {
    }
    auto Mark() -> float
    {
        const auto old = last;
        last = std::chrono::steady_clock::now();
        const std::chrono::duration<float> duration = last - old;
        return duration.count();
    }
    auto Peek() -> float
    {
        const std::chrono::duration<float> duration = std::chrono::steady_clock::now() - last;
        return duration.count();
    }
};
