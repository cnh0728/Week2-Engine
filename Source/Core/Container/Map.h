#pragma once
#include <unordered_map>
#include <utility>
#include <algorithm>
#include "ContainerAllocator.h"
#include "Pair.h"


template <typename KeyType, typename ValueType, typename Allocator = FDefaultAllocator<std::pair<const KeyType, ValueType>>>
class TMap
{
public:
    using PairType = TPair<const KeyType, ValueType>;
    using MapType = std::unordered_map<KeyType, ValueType, std::hash<KeyType>, std::equal_to<KeyType>, Allocator>;
    using SizeType = typename MapType::size_type;

private:
    MapType PrivateMap;

    class Iterator
    {
    private:
        typename MapType::iterator InnerIt;
    public:
        Iterator(typename MapType::iterator it) : InnerIt(it) {}
        PairType& operator*() { return reinterpret_cast<PairType&>(*InnerIt); }
        PairType* operator->() { return reinterpret_cast<PairType*>(&(*InnerIt)); }
        Iterator& operator++() { ++InnerIt; return *this; }
        bool operator!=(const Iterator& other) const { return InnerIt != other.InnerIt; }
    };

    class ConstIterator
    {
    private:
        typename MapType::const_iterator InnerIt;
    public:
        ConstIterator(typename MapType::const_iterator it) : InnerIt(it) {}
        const PairType& operator*() const { return reinterpret_cast<const PairType&>(*InnerIt); }
        const PairType* operator->() const { return reinterpret_cast<const PairType*>(&(*InnerIt)); }
        ConstIterator& operator++() { ++InnerIt; return *this; }
        bool operator!=(const ConstIterator& other) const { return InnerIt != other.InnerIt; }
    };

public:
    // TPair를 반환하는 커스텀 반복자
    Iterator begin() noexcept { return Iterator(PrivateMap.begin()); }
    Iterator end() noexcept { return Iterator(PrivateMap.end()); }
    ConstIterator begin() const noexcept { return ConstIterator(PrivateMap.begin()); }
    ConstIterator end() const noexcept { return ConstIterator(PrivateMap.end()); }

    // 생성자 및 소멸자
    TMap() = default;
    ~TMap() = default;

    // 복사 생성자
    TMap(const TMap& Other) : PrivateMap(Other.PrivateMap) {}

    // 이동 생성자
    TMap(TMap&& Other) noexcept : PrivateMap(std::move(Other.PrivateMap)) {}

    // 초기화 생성자
    TMap(std::initializer_list<std::pair<const KeyType, ValueType>> InitList)
    : PrivateMap(InitList) {}
    
    // 복사 할당 연산자
    TMap& operator=(const TMap& Other)
    {
        if (this != &Other)
        {
            PrivateMap = Other.PrivateMap;
        }
        return *this;
    }

    // 이동 할당 연산자
    TMap& operator=(TMap&& Other) noexcept
    {
        if (this != &Other)
        {
            PrivateMap = std::move(Other.PrivateMap);
        }
        return *this;
    }

    // 요소 접근 및 수정
    ValueType& operator[](const KeyType& Key)
    {
        return PrivateMap[Key];
    }

    const ValueType& operator[](const KeyType& Key) const
    {
        return PrivateMap.at(Key);
    }

    void Add(const KeyType& Key, const ValueType& Value)
    {
        PrivateMap.insert_or_assign(Key, Value);
    }

    void Emplace(KeyType&& Key, ValueType&& Value)
    {
        PrivateMap.emplace(std::move(Key), std::move(Value));
    }

    void Remove(const KeyType& Key)
    {
        PrivateMap.erase(Key);
    }

    void Empty()
    {
        PrivateMap.clear();
    }

    // 검색 및 조회
    bool Contains(const KeyType& Key) const
    {
        return PrivateMap.find(Key) != PrivateMap.end();
    }

    const ValueType* Find(const KeyType& Key) const
    {
        auto it = PrivateMap.find(Key);
        return it != PrivateMap.end() ? &(it->second) : nullptr;
    }

    ValueType* Find(const KeyType& Key)
    {
        auto it = PrivateMap.find(Key);
        return it != PrivateMap.end() ? &(it->second) : nullptr;
    }

    // 크기 관련
    SizeType Num() const
    {
        return PrivateMap.size();
    }

    bool IsEmpty() const
    {
        return PrivateMap.empty();
    }

    // 용량 관련
    void Reserve(SizeType Number)
    {
        PrivateMap.reserve(Number);
    }
};
