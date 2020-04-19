#pragma once

#include <string>
#include <cassert>

inline auto CharToWString(const char* c) -> std::wstring
{
    size_t outputSize = strlen(c);
    std::wstring wstr(outputSize, L'#');

    size_t charConverted{};
    const errno_t errNo = mbstowcs_s(&charConverted, wstr.data(), wstr.size() + 1, c, outputSize);

    assert(errNo == 0);
    assert(charConverted == wstr.size() + 1);

    return wstr;
}

inline auto WCharToString(const wchar_t* wc) -> std::string
{
    size_t outputSize = wcslen(wc);
    std::string str(outputSize, '#');

    size_t charConverted{};
    const errno_t errNo = wcstombs_s(&charConverted, str.data(), str.size() + 1, wc, outputSize);

    assert(errNo == 0);
    assert(charConverted == str.size() + 1);

    return str;
}