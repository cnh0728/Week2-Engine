#pragma once
#include <type_traits>
#include <concepts>
//#include "UObject.h"
#include "Class/Class.h"

class UObject;

template <typename T>
concept IsUObjectDerived = std::is_base_of_v<UObject, T>;

template <typename To, typename From>
concept BothAreUObjects = IsUObjectDerived<To> && IsUObjectDerived<From>;

    //requires BothAreUObjects<To, From>
template <typename To, typename From>
    requires (std::is_base_of_v<UObject, To> && std::is_base_of_v<UObject, From>)
FORCEINLINE To* Cast(From* Src)
{
    static_assert(sizeof(From) > 0 && sizeof(To) > 0, "Attempting to cast between incomplete types");

    if (!Src) return nullptr;

    return Src->IsA(To::StaticClass()) ? static_cast<To*>(Src) : nullptr;
}

    //requires BothAreUObjects<To, From>
template <typename To, typename From>
    requires (std::is_base_of_v<UObject, To>&& std::is_base_of_v<UObject, From>)
FORCEINLINE const To* Cast(const From* Src)
{
    static_assert(sizeof(From) > 0 && sizeof(To) > 0, "Attempting to cast between incomplete types");

    if (!Src) return nullptr;

    return Src->IsA(To::StaticClass()) ? static_cast<To*>(Src) : nullptr;
}


