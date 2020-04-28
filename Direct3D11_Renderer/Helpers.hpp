#pragma once

#include <string>
#include <cassert>

inline auto CharToWString(const char* c) -> std::wstring
{
    size_t outputSize = strlen(c);
    std::wstring wstr(outputSize, L'#');

    const errno_t err = mbstowcs_s(nullptr, wstr.data(), wstr.size() + 1, c, _TRUNCATE);

    if (err == STRUNCATE)
    {
        OutputDebugString(L"Truncation occurred on string: \"");
        OutputDebugStringA(c);
        OutputDebugString(L"\"!\n");
    }

    return wstr;
}

inline auto WCharToString(const wchar_t* wc) -> std::string
{
    size_t outputSize = wcslen(wc);
    std::string str(outputSize, '#');

    const errno_t err = wcstombs_s(nullptr, str.data(), str.size() + 1, wc, _TRUNCATE);

    if (err == STRUNCATE)
    {
        OutputDebugString(L"Truncation occurred on string: \"");
        OutputDebugString(wc);
        OutputDebugString(L"\"!\n");
    }

    return str;
}