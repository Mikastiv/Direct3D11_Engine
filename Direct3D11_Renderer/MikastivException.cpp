#include "MikastivException.hpp"

#include <sstream>

MikastivException::MikastivException(const int line, const char* file) noexcept
    : line(line)
    , file(file)
{
}

auto MikastivException::what() const noexcept -> const char*
{
    std::ostringstream oss;
    oss << get_type() << '\n' << get_origin_string();
    what_buffer = oss.str();

    return what_buffer.c_str();
}

auto MikastivException::get_type() const noexcept -> const char*
{
    return "Mikastiv Exception";
}

auto MikastivException::get_line() const noexcept -> int
{
    return line;
}

auto MikastivException::get_file() const noexcept -> const std::string&
{
    return file;
}

auto MikastivException::get_origin_string() const noexcept -> std::string
{
    std::ostringstream oss{};
    oss << "[File] " << file << '\n' << "[Line] " << line;
    return oss.str();
}
