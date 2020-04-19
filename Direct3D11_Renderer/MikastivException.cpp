#include "MikastivException.hpp"

#include <sstream>

MikastivException::MikastivException(int line, const char* file) noexcept
    : line(line)
    , file(file)
{
}

auto MikastivException::what() const noexcept -> const char*
{
    std::ostringstream oss;
    oss << GetType() << '\n' << GetOriginString();
    whatBuffer = oss.str();

    return whatBuffer.c_str();
}

auto MikastivException::GetType() const noexcept -> const char*
{
    return "Mikastiv Exception";
}

auto MikastivException::GetLine() const noexcept -> int
{
    return line;
}

auto MikastivException::GetFile() const noexcept -> const std::string&
{
    return file;
}

auto MikastivException::GetOriginString() const noexcept -> std::string
{
    std::ostringstream oss{};
    oss << "[File] " << file << '\n' << "[Line] " << line;
    return oss.str();
}
