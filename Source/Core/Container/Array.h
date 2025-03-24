#pragma once
#include <algorithm>
#include <utility>
#include <vector>

#include "ContainerAllocator.h"


template <typename T, typename Allocator>
class TArray
{
public:
    using SizeType = typename Allocator::SizeType;

private:
    std::vector<T, Allocator> PrivateVector;

public:
    // Iterator를 사용하기 위함
    auto begin() noexcept { return PrivateVector.begin(); }
    auto end() noexcept { return PrivateVector.end(); }
    auto begin() const noexcept { return PrivateVector.begin(); }
    auto end() const noexcept { return PrivateVector.end(); }
    auto rbegin() noexcept { return PrivateVector.rbegin(); }
    auto rend() noexcept { return PrivateVector.rend(); }
    auto rbegin() const noexcept { return PrivateVector.rbegin(); }
    auto rend() const noexcept { return PrivateVector.rend(); }

    T& operator[](SizeType Index);
    const T& operator[](SizeType Index) const;

public:
    TArray();
    ~TArray() = default;

    // 복사 생성자
    TArray(const TArray& Other);

    // 이동 생성자
    TArray(TArray&& Other) noexcept;

    // 초기화 생성자
    TArray(std::initializer_list<T> InitList);

    // 복사 할당 연산자
    TArray& operator=(const TArray& Other);

    // 이동 할당 연산자
    TArray& operator=(TArray&& Other) noexcept;

    void Init(const T& Element, SizeType Number);
    SizeType Add(const T& Item);
    SizeType Add(T&& Item);
    SizeType AddUnique(const T& Item);
    SizeType Emplace(T&& Item);
    void Empty();
    SizeType Remove(const T& Item);
    bool RemoveSingle(const T& Item);
    void RemoveAt(SizeType Index);
    template <typename Predicate>
        requires std::is_invocable_r_v<bool, Predicate, const T&>
    SizeType RemoveAll(const Predicate& Pred);
    T* GetData();
    T* ToArray() const;
    
    SizeType Insert(SizeType Index, const T& Item);
    SizeType Insert(SizeType Index, T&& Item);
    void Insert(SizeType Index, const T* Items, SizeType Count);
    void Insert(SizeType Index, std::initializer_list<T> InitList);
    template <typename InputIt>
    void Insert(typename std::vector<T, Allocator>::iterator Position, InputIt First, InputIt Last);
    void Resize(SizeType NewSize);
    void Resize(SizeType NewSize, const T& Value);

    /**
     * Array에서 Item을 찾습니다.
     * @param Item 찾으려는 Item
     * @return Item의 인덱스, 찾을 수 없다면 -1
     */
    SizeType Find(const T& Item);
    bool Find(const T& Item, SizeType& Index);

    /** Size */
    SizeType Num() const;

    /** Capacity */
    SizeType Len() const;

    void Reserve(SizeType Number);

    void Sort();
    template <typename Compare>
        requires std::is_invocable_r_v<bool, Compare, const T&, const T&>
    void Sort(const Compare& CompFn);
};


template <typename T, typename Allocator>
T& TArray<T, Allocator>::operator[](SizeType Index)
{
    return PrivateVector[Index];
}

template <typename T, typename Allocator>
const T& TArray<T, Allocator>::operator[](SizeType Index) const
{
    return PrivateVector[Index];
}

template <typename T, typename Allocator>
TArray<T, Allocator>::TArray(): PrivateVector()
{
}

template <typename T, typename Allocator>
TArray<T, Allocator>::TArray(const TArray& Other): PrivateVector(Other.PrivateVector)
{
}

template <typename T, typename Allocator>
TArray<T, Allocator>::TArray(TArray&& Other) noexcept: PrivateVector(std::move(Other.PrivateVector))
{
}

template <typename T, typename Allocator>
TArray<T, Allocator>& TArray<T, Allocator>::operator=(const TArray& Other)
{
    if (this != &Other)
    {
        PrivateVector = Other.PrivateVector;
    }
    return *this;
}

template <typename T, typename Allocator>
TArray<T, Allocator>& TArray<T, Allocator>::operator=(TArray&& Other) noexcept
{
    if (this != &Other)
    {
        PrivateVector = std::move(Other.PrivateVector);
    }
    return *this;
}

template <typename T, typename Allocator>
void TArray<T, Allocator>::Init(const T& Element, SizeType Number)
{
    PrivateVector.assign(Number, Element);
}

template <typename T, typename Allocator>
typename TArray<T, Allocator>::SizeType TArray<T, Allocator>::Add(const T& Item)
{
    return Emplace(T(Item));
}

template <typename T, typename Allocator>
typename TArray<T, Allocator>::SizeType TArray<T, Allocator>::Add(T&& Item)
{
    return Emplace(std::move(Item));
}

template <typename T, typename Allocator>
typename TArray<T, Allocator>::SizeType TArray<T, Allocator>::AddUnique(const T& Item)
{
    if (SizeType Index; Find(Item, Index))
    {
        return Index;
    }
    return Add(Item);
}

template <typename T, typename Allocator>
typename TArray<T, Allocator>::SizeType TArray<T, Allocator>::Emplace(T&& Item)
{
    PrivateVector.emplace_back(std::move(Item));
    return Num()-1;
}

template <typename T, typename Allocator>
void TArray<T, Allocator>::Empty()
{
    PrivateVector.clear();
}

template <typename T, typename Allocator>
typename TArray<T, Allocator>::SizeType TArray<T, Allocator>::Remove(const T& Item)
{
    auto oldSize = PrivateVector.size();
    PrivateVector.erase(std::remove(PrivateVector.begin(), PrivateVector.end(), Item), PrivateVector.end());
    return static_cast<SizeType>(oldSize - PrivateVector.size());
}

template <typename T, typename Allocator>
bool TArray<T, Allocator>::RemoveSingle(const T& Item)
{
    auto it = std::find(PrivateVector.begin(), PrivateVector.end(), Item);
    if (it != PrivateVector.end())
    {
        PrivateVector.erase(it);
        return true;
    }
    return false;
}

template <typename T, typename Allocator>
void TArray<T, Allocator>::RemoveAt(SizeType Index)
{
    if (Index >= 0 && static_cast<SizeType>(Index) < PrivateVector.size())
    {
        PrivateVector.erase(PrivateVector.begin() + Index);
    }
}

template <typename T, typename Allocator>
template <typename Predicate>
    requires std::is_invocable_r_v<bool, Predicate, const T&>
typename TArray<T, Allocator>::SizeType TArray<T, Allocator>::RemoveAll(const Predicate& Pred)
{
    auto oldSize = PrivateVector.size();
    PrivateVector.erase(std::remove_if(PrivateVector.begin(), PrivateVector.end(), Pred), PrivateVector.end());
    return static_cast<SizeType>(oldSize - PrivateVector.size());
}

template <typename T, typename Allocator>
T* TArray<T, Allocator>::GetData()
{
    return PrivateVector.data();
}

template <typename T, typename Allocator>
T* TArray<T, Allocator>::ToArray() const
{
    SizeType size = Num();
    if (size == 0)
    {
        return nullptr;
    }

    T* newArray = new T[size];
    std::copy(PrivateVector.begin(), PrivateVector.end(), newArray);
    return newArray;
}

template <typename T, typename Allocator>
typename TArray<T, Allocator>::SizeType TArray<T, Allocator>::Find(const T& Item)
{
    const auto it = std::find(PrivateVector.begin(), PrivateVector.end(), Item);
    return it != PrivateVector.end() ? std::distance(PrivateVector.begin(), it) : -1;
}

template <typename T, typename Allocator>
bool TArray<T, Allocator>::Find(const T& Item, SizeType& Index)
{
    Index = Find(Item);
    return (Index != -1);
}

template <typename T, typename Allocator>
typename TArray<T, Allocator>::SizeType TArray<T, Allocator>::Num() const
{
    return PrivateVector.size();
}

template <typename T, typename Allocator>
typename TArray<T, Allocator>::SizeType TArray<T, Allocator>::Len() const
{
    return PrivateVector.capacity();
}

template <typename T, typename Allocator>
void TArray<T, Allocator>::Reserve(SizeType Number)
{
    PrivateVector.reserve(Number);
}

template <typename T, typename Allocator>
void TArray<T, Allocator>::Sort()
{
    std::sort(PrivateVector.begin(), PrivateVector.end());
}

template <typename T, typename Allocator>
template <typename Compare>
    requires std::is_invocable_r_v<bool, Compare, const T&, const T&>
void TArray<T, Allocator>::Sort(const Compare& CompFn)
{
    std::sort(PrivateVector.begin(), PrivateVector.end(), CompFn);
}

template <typename T, typename Allocator = FDefaultAllocator<T>> class TArray;

template <typename T, typename Allocator>
TArray<T, Allocator>::TArray(std::initializer_list<T> InitList)
    : PrivateVector(InitList) {}
    
template <typename T, typename Allocator>
typename TArray<T, Allocator>::SizeType TArray<T, Allocator>::Insert(SizeType Index, const T& Item)
{
    if (Index >= 0 && static_cast<SizeType>(Index) <= PrivateVector.size())
    {
        PrivateVector.insert(PrivateVector.begin() + Index, Item);
        return Index;
    }
    throw std::out_of_range("Index out of range in TArray::Insert");
}

template <typename T, typename Allocator>
typename TArray<T, Allocator>::SizeType TArray<T, Allocator>::Insert(SizeType Index, T&& Item)
{
    if (Index >= 0 && static_cast<SizeType>(Index) <= PrivateVector.size())
    {
        PrivateVector.insert(PrivateVector.begin() + Index, std::move(Item));
        return Index;
    }
    throw std::out_of_range("Index out of range in TArray::Insert");
}

template <typename T, typename Allocator>
void TArray<T, Allocator>::Insert(SizeType Index, const T* Items, SizeType Count)
{
    if (Index >= 0 && static_cast<SizeType>(Index) <= PrivateVector.size())
    {
        PrivateVector.insert(PrivateVector.begin() + Index, Items, Items + Count);
        return;
    }
    throw std::out_of_range("Index out of range in TArray::Insert");
}

template <typename T, typename Allocator>
void TArray<T, Allocator>::Insert(SizeType Index, std::initializer_list<T> InitList)
{
    if (Index >= 0 && static_cast<SizeType>(Index) <= PrivateVector.size())
    {
        PrivateVector.insert(PrivateVector.begin() + Index, InitList.begin(), InitList.end());
        return;
    }
    throw std::out_of_range("Index out of range in TArray::Insert");
}


template <typename T, typename Allocator>
template <typename InputIt>
void TArray<T, Allocator>::Insert(typename std::vector<T, Allocator>::iterator Position, InputIt First, InputIt Last)
{
    PrivateVector.insert(Position, First, Last);
}

template <typename T, typename Allocator>
void TArray<T, Allocator>::Resize(SizeType NewSize)
{
    if (NewSize < 0)
    {
        throw std::invalid_argument("Negative size in TArray::Resize");
    }
    PrivateVector.resize(static_cast<typename std::vector<T, Allocator>::size_type>(NewSize));
}

template <typename T, typename Allocator>
void TArray<T, Allocator>::Resize(SizeType NewSize, const T& Value)
{
    if (NewSize < 0)
    {
        throw std::invalid_argument("Negative size in TArray::Resize");
    }
    PrivateVector.resize(
        static_cast<typename std::vector<T, Allocator>::size_type>(NewSize), 
        Value
    );
}