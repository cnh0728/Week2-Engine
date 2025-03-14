#pragma once

#include <string>
#include "CString.h"
#include "ContainerAllocator.h"
#include "Core/HAL/PlatformType.h"
#include <functional>

/*
# TCHAR가 ANSICHAR인 경우
1. const ANSICHAR* 로 FString 생성
2. std::string에서 FString 생성

# TCHAR가 WIDECHAR인 경우
1. const ANSICHAR* 로 FString 생성
1. const WIDECHAR* 로 FString 생성
2. std::wstring에서 FString 생성
3. std::string에서 FString 생성
*/

enum : int8 { INDEX_NONE = -1 };

/** Determines case sensitivity options for string comparisons. */
namespace ESearchCase
{
enum Type : uint8
{
    /** Case sensitive. Upper/lower casing must match for strings to be considered equal. */
    CaseSensitive,

    /** Ignore case. Upper/lower casing does not matter when making a comparison. */
    IgnoreCase,
};
};

/** Determines search direction for string operations. */
namespace ESearchDir
{
enum Type : uint8
{
    /** Search from the start, moving forward through the string. */
    FromStart,

    /** Search from the end, moving backward through the string. */
    FromEnd,
};
}

class FString
{
private:
    using BaseStringType = std::basic_string<
        TCHAR,
        std::char_traits<TCHAR>,
        FDefaultAllocator<TCHAR>
    >;

    BaseStringType PrivateString;

public:
    FString() = default;
    ~FString() = default;

    FString(const FString&) = default;
    FString& operator=(const FString&) = default;
    FString(FString&&) = default;
    FString& operator=(FString&&) = default;

    FString(BaseStringType InString) : PrivateString(std::move(InString)) {}

#if IS_WIDECHAR
private:
    static std::wstring ConvertWideChar(const ANSICHAR* NarrowStr);

public:
    FString(const std::wstring& InString) : PrivateString(InString) {}
    FString(const std::string& InString) : PrivateString(ConvertWideChar(InString.c_str())) {}
    FString(const WIDECHAR* InString) : PrivateString(InString) {}
    FString(const ANSICHAR* InString) : PrivateString(ConvertWideChar(InString)) {}
#else
public:
    FString(const std::string& InString) : PrivateString(InString) {}
    FString(const ANSICHAR* InString) : PrivateString(InString) {}
#endif

    static FString FromInt(int32 Num);
    static FString SanitizeFloat(float InFloat);

public:
    FORCEINLINE int32 Len() const;
    FORCEINLINE bool IsEmpty() const;

    /** 배열의 모든 요소를 지웁니다. */
    void Empty();

    /** 왼쪽, 오른쪽에서 Count개의 문자를 잘라낸 새 FString 반환 */
    FORCEINLINE FString Left(int32 Count) const;
    FORCEINLINE FString Right(int32 Count) const;

    /** 좌우 공백을 제거한 새 FString 반환 **/
    FORCEINLINE FString Trim() const;

    /**
     * 문자열이 서로 같은지 비교합니다.
     * @param Other 비교할 String
     * @param SearchCase 대소문자 구분
     * @return 같은지 여부
     */
    bool Equals(const FString& Other, ESearchCase::Type SearchCase = ESearchCase::CaseSensitive) const;

    /**
     * 문자열이 겹치는지 확인합니다.
     * @param SubStr 찾을 문자열
     * @param SearchCase 대소문자 구분
     * @param SearchDir 찾을 방향
     * @return 문자열 겹침 여부
     */
    bool Contains(
        const FString& SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase,
        ESearchDir::Type SearchDir = ESearchDir::FromStart
    ) const;

    /**
     * 문자열을 찾아 Index를 반홥합니다.
     * @param SubStr 찾을 문자열
     * @param SearchCase 대소문자 구분
     * @param SearchDir 찾을 방향
     * @param StartPosition 시작 위치
     * @return 찾은 문자열의 Index를 반환합니다. 찾지 못하면 -1
     */
    int32 Find(
        const FString& SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase,
        ESearchDir::Type SearchDir = ESearchDir::FromStart, int32 StartPosition = -1
    ) const;

public:
    /** TCHAR* 로 반환하는 연산자 */
    FORCEINLINE const TCHAR* operator*() const;

    //FORCEINLINE FString operator+(const FString& SubStr) const;
    FORCEINLINE FString& operator+=(const FString& SubStr);
    FORCEINLINE friend FString operator+(const FString& Lhs, const FString& Rhs);

    FORCEINLINE bool operator==(const FString& Rhs) const;
    FORCEINLINE bool operator==(const TCHAR* Rhs) const;
};


FORCEINLINE int32 FString::Len() const
{
    return static_cast<int32>(PrivateString.length());
}

FORCEINLINE bool FString::IsEmpty() const
{
    return PrivateString.empty();
}

FORCEINLINE FString FString::Left(int32 Count) const
{
    if (Count < 0)
    {
        return FString();
    }
    if (Count > Len())
    {
        Count = Len();
    }
    return FString(BaseStringType(PrivateString.begin(), PrivateString.begin() + Count));
}

FORCEINLINE FString FString::Right(int32 Count) const
{
    if (Count < 0)
    {
        return FString();
    }
    if (Count > Len())
    {
        Count = Len();
    }
    return FString(BaseStringType(PrivateString.end() - Count, PrivateString.end()));
}

FORCEINLINE FString FString::Trim() const
{
    auto IsWhiteSpace = [](TCHAR c) -> bool {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
        };

    auto start = std::find_if(PrivateString.begin(), PrivateString.end(), [&](TCHAR c) {
        return !IsWhiteSpace(c);
        });

    auto end = std::find_if(PrivateString.rbegin(), PrivateString.rend(), [&](TCHAR c) {
        return !IsWhiteSpace(c);
        }).base();

    if (start >= end)
    {
        return FString();
    }
    return FString(BaseStringType(start, end));
}

FORCEINLINE const TCHAR* FString::operator*() const
{
    return PrivateString.c_str();
}

FString operator+(const FString& Lhs, const FString& Rhs)
{
    FString CopyLhs{Lhs};
    return CopyLhs += Rhs;
}

FORCEINLINE bool FString::operator==(const FString& Rhs) const
{
    return Equals(Rhs, ESearchCase::IgnoreCase);
}

FORCEINLINE bool FString::operator==(const TCHAR* Rhs) const
{
    return Equals(Rhs);
}

FORCEINLINE FString& FString::operator+=(const FString& SubStr)
{
    this->PrivateString += SubStr.PrivateString;
    return *this;
}

namespace std {
    template <>
    struct hash<FString>
    {
        size_t operator()(const FString& s) const
        {
            return std::hash<std::string>()(std::string(*s));
        }
    };
}
