#pragma once

#include <exception>
#include <string>

class MikastivException : public std::exception
{
private:
    int line{};
    std::string file{};

protected:
    mutable std::string what_buffer{};

public:
    MikastivException(const int line, const char* file) noexcept;
    const char* what() const noexcept override;
    virtual auto get_type() const noexcept -> const char*;
    auto get_line() const noexcept -> int;
    auto get_file() const noexcept -> const std::string&;
    auto get_origin_string() const noexcept -> std::string;
};
