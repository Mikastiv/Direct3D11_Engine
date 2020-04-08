#pragma once

#include <string>
#include <cassert>

inline auto char_to_wstr(const char* c) -> std::wstring
{
    size_t str_size = strlen(c);
    std::wstring wc(str_size, L'#');

    size_t out_size{};
    const errno_t err_no = mbstowcs_s(&out_size, wc.data(), wc.size() + 1, c, str_size);

    assert(err_no == 0);

    return wc;
}