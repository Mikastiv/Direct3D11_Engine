#pragma once

#include <exception>
#include <string>

class MikastivException : public std::exception
{
public:
    MikastivException(int line, const char* file) noexcept;
    auto what() const noexcept -> const char* override;
    virtual auto GetType() const noexcept -> const char*;
    auto GetLine() const noexcept -> int;
    auto GetFile() const noexcept -> const std::string&;
    auto GetOriginString() const noexcept -> std::string;

protected:
    mutable std::string whatBuffer{};

private:
    int line = 0;
    std::string file{};
};
