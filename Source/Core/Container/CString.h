#pragma once
#include <cstring>
#include <cwchar>
#include <cctype>
#include <type_traits>

template <typename T>
constexpr int UnsupportedCharacterType()
{
    static_assert(sizeof(T) == 0, "Unsupported character type!");
    return 0;
}


template <typename T>
struct TCString
{
public:
    using CharType = T;

    // StrCmp method for comparing two strings
    static int StrCmp(const CharType* str1, const CharType* str2)
    {
        if constexpr (std::is_same_v<CharType, char>)
        {
            // Use std::strcmp for char strings
            return std::strcmp(str1, str2);
        }
        else if constexpr (std::is_same_v<CharType, wchar_t>)
        {
            // Use std::wcscmp for wchar_t strings
            return std::wcscmp(str1, str2);
        }
        else
        {
            return UnsupportedCharacterType<CharType>();
        }
    }

    static CharType* Strcpy(CharType* dest, const CharType* src)
    {
        if constexpr (std::is_same_v<CharType, char>)
        {
            return std::strcpy(dest, src);
        }
        else if constexpr (std::is_same_v<CharType, wchar_t>)
        {
            return std::wcscpy(dest, src);
        }
        else
        {
            return UnsupportedCharacterType<CharType>();
        }
    }

    static CharType* Strncpy(CharType* dest, const CharType* src, std::size_t count)
    {
        if constexpr (std::is_same_v<CharType, char>)
        {
            return std::strncpy(dest, src, count);
        }
        else if constexpr (std::is_same_v<CharType, wchar_t>)
        {
            return std::wcsncpy(dest, src, count);
        }
        else
        {
            return UnsupportedCharacterType<CharType>();
        }
    }

    static CharType* Strcat(CharType* dest, const CharType* src)
    {
        if constexpr (std::is_same_v<CharType, char>)
        {
            return std::strcat(dest, src);
        }
        else if constexpr (std::is_same_v<CharType, wchar_t>)
        {
            return std::wcscat(dest, src);
        }
        else
        {
            return UnsupportedCharacterType<CharType>();
        }
    }

    static CharType* Strupr(CharType* str)
    {
        CharType* p = str;
        while (*p)
        {
            *p = std::toupper(static_cast<unsigned char>(*p));
            ++p;
        }
        return str;
    }

    static int Strcmp(const CharType* str1, const CharType* str2)
    {
        if constexpr (std::is_same_v<CharType, char>)
        {
            return std::strcmp(str1, str2);
        }
        else if constexpr (std::is_same_v<CharType, wchar_t>)
        {
            return std::wcscmp(str1, str2);
        }
        else
        {
            return UnsupportedCharacterType<CharType>();
        }
    }

    static int Strncmp(const CharType* str1, const CharType* str2, std::size_t count)
    {
        if constexpr (std::is_same_v<CharType, char>)
        {
            return std::strncmp(str1, str2, count);
        }
        else if constexpr (std::is_same_v<CharType, wchar_t>)
        {
            return std::wcsncmp(str1, str2, count);
        }
        else
        {
            return UnsupportedCharacterType<CharType>();
        }
    }

    static int Stricmp(const CharType* str1, const CharType* str2)
    {
        while (*str1 && (std::tolower(static_cast<unsigned char>(*str1)) == std::tolower(static_cast<unsigned char>(*str2))))
        {
            ++str1;
            ++str2;
        }
        return std::tolower(static_cast<unsigned char>(*str1)) - std::tolower(static_cast<unsigned char>(*str2));
    }

    static int Strnicmp(const CharType* str1, const CharType* str2, std::size_t count)
    {
        while (count-- && *str1 && (std::tolower(static_cast<unsigned char>(*str1)) == std::tolower(static_cast<unsigned char>(*str2))))
        {
            ++str1;
            ++str2;
        }
        return count ? std::tolower(static_cast<unsigned char>(*str1)) - std::tolower(static_cast<unsigned char>(*str2)) : 0;
    }
};
