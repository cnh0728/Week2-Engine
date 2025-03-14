#pragma once
#include <unordered_map>
#include "Core/Container/String.h"
//#include "Core/Container/Map.h"
#include "Core/AbstractClass/Singleton.h"

// FNamePool에 저장할 때 중복확인하기위한 코드
struct FNameEntryHeader
{
	uint16 bIsWide : 1 = 1;                  // 1비트 (Wide 여부). 저장되었는지 확인하는 여부로도 이용가능(나머지가 0일수 있진않지만,,,)
	uint16 LowercaseProbeHash : 5;		 // Hash값의 하위 5비트만 저장
	uint16 Len : 10;					 // FNameEntry의 길이

	FORCEINLINE bool operator==(const FNameEntryHeader& Other) const;
	FORCEINLINE bool operator==(const uint16 BitPattern) const;
};


struct FNameEntry
{
	FNameEntryHeader Header;
	FString Data;

	FORCEINLINE uint32 GetNameLength() const { return Header.Len; }
	FORCEINLINE uint32 GetProbeHash() const { return Header.LowercaseProbeHash; }
	FORCEINLINE uint32 GetIsWide() const { return Header.bIsWide; }

	FNameEntry& operator=(const FNameEntry&) = default;
	FNameEntry& operator=(FNameEntry&&) = default;

	bool operator==(const FNameEntry& Other);
	bool operator!=(const FNameEntry& Other);

	FORCEINLINE bool isEmpty() const { return Header == 0; }

private:
};

/////////////////////////
/// "Player"  -> Number = 0 
/// "Player0" -> Number = 1 
/// "Player1" -> Number = 2
struct FName
{
	FName(char* pStr) : FName(FString(pStr)) {}
	FName(FString str);
	FName() = delete;

	FORCEINLINE int32 Compare(const FName& Other) const;
	FORCEINLINE bool operator==(const FName& Other) const;

	//  "Player13"이면,
	// ComparisonIndex = ("Player" 문자열의 EntryId == hash value)
	// Number = 14 ( trailing number가 없으면 0으로 처리함. 따라서 숫자가 붙은 문자열이 들어올 경우 +1로 처리)
	uint32 DisplayIndex; // 실제 value가 들어가있는 index
	uint32 ComparisonIndex; // 비교용 index
	uint32 Number; // 숫자를 의미
};


// hash function을 FString에도 이용가능하게 변경
template <>
struct std::hash<FString>
{
	std::size_t operator()(const FString& Str) const
	{
		return Str.GetHash();
	}
};

class FNamePool : public TSingleton<FNamePool>
{
public:
	FNamePool() {}
	uint32 FindOrAddComparisonName(FString ComparisonName);
	uint32 FindOrAddDisplayName(FString DisplayName);
	FString ResolveComparison(uint32 Id) const;
	FString ResolveDisplay(uint32 Id) const;

//private:
	static constexpr std::hash<FString> Hasher;
	//TMap<uint32, FNameEntry> Entries;

	
	std::hash<FString> ProbeHasher;

	static constexpr uint32 PoolSize = 1024;
	uint32 CurrentComparisonPoolSize = 0;
	uint32 CurrentDisplayPoolSize = 0;
	static FNameEntry ComparisonNameEntryPool[PoolSize]; // bIsWide도 0으로 초기화. 메모리공간이 다 0으로 참
	static FNameEntry DisplayNameEntryPool[PoolSize];


	static constexpr uint8 CacheSize = 16;
	uint32 ComparisonNameEntryCache[CacheSize][2] = {0}; // hash값 / ComparisonNameEntryPool의 Id값 : 
	uint32 DisplayNameEntryCache[CacheSize][2] = {0};
	uint8 ComparisonCacheCursor = 0;
	uint8 DisplayCacheCursor = 0;

	bool FindComparisonNameCached(uint32 InHash, uint32& OutId);
	bool FindDisplayNameCached(uint32 InHash, uint32& OutId);
};