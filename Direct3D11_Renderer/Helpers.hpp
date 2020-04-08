#pragma once

#include <string>
#include <cassert>

inline auto char_to_wstr(const char* c) -> std::wstring
{
    size_t output_size = strlen(c);
    std::wstring wstr(output_size, L'#');

    size_t char_converted{};
    const errno_t err_no = mbstowcs_s(&char_converted, wstr.data(), wstr.size() + 1, c, output_size);

    assert(err_no == 0);
    assert(char_converted == wstr.size() + 1);

    return wstr;
}

inline auto wchar_to_str(const wchar_t* wc) -> std::string
{
    size_t output_size = wcslen(wc);
    std::string str(output_size, '#');

    size_t char_converted{};
    const errno_t err_no = wcstombs_s(&char_converted, str.data(), str.size() + 1, wc, output_size);

    assert(err_no == 0);
    assert(char_converted == str.size() + 1);

    return str;
}