#include "Name.h"
#include "Debug/DebugConsole.h"

FNameEntry FNamePool::ComparisonNameEntryPool[PoolSize] = { 0 };
FNameEntry FNamePool::DisplayNameEntryPool[PoolSize] = { 0 };

// 오직 알파벳만 있어야하고, 빈 string이 아니여야함
// lowercase여야함
uint32 FNamePool::FindOrAddComparisonName(FString ComparisonName)
{
	//FString InNameStringPartLower = InDisplayName.ToLower();
	uint32 InHash = ComparisonName.GetHash() % PoolSize; // pool size에 맞게 조절
	
	// 캐시 비교
	// cache 확인
	uint32 PossibleIdCached;
	if (FindComparisonNameCached(InHash, PossibleIdCached))
	{
		if (ComparisonName == ResolveComparison(PossibleIdCached))
		{
			return PossibleIdCached;
		}
	}


	FNameEntryHeader InHeader;
	InHeader.LowercaseProbeHash = InHash && uint16(31); // 빠른 비교를 위한 5비트 tag
	InHeader.Len = ComparisonName.Len(); // 빠른 비교를 위한 길이 tag
	
	FNameEntry InEntry;
	InEntry.Data = ComparisonName;
	InEntry.Header = InHeader;


#ifdef IS_FNAME_POOL_HASHED
	uint32 ComparisonIndex = InHash;
	do
	{
		if (ComparisonNameEntryPool[ComparisonIndex].isEmpty()) // 빈곳을 찾음
		{
			ComparisonNameEntryPool[ComparisonIndex] = InEntry;
			return ComparisonIndex;
		}
		if (ComparisonNameEntryPool[ComparisonIndex] == InEntry) //	비어있지 않음 -> 같은 값인지 확인
		{
			// 찾음 : 2번이상 저장하면 Cache에 저장
			if (ComparisonNameEntryCache[ComparisonCacheCursor][1] != ComparisonIndex)
			{
				ComparisonCacheCursor = (ComparisonCacheCursor + 1) % CacheSize;
				ComparisonNameEntryCache[ComparisonCacheCursor][0] = InHash;
				ComparisonNameEntryCache[ComparisonCacheCursor][1] = ComparisonIndex;
			}
			return ComparisonIndex;
		}
		ComparisonIndex = (ComparisonIndex + 1) % PoolSize;
	} while (ComparisonIndex != InHash);
	
	UE_LOG("FName::ComparisonNameEntryPool is full.\n");
	return 0;

#else
	if (CurrentComparisonPoolSize == PoolSize)
	{
		UE_LOG("FName::ComparisonNameEntryPool is full.\n");
		return 0;
	}

	//constexpr uint32 stride = 64 / sizeof(FNameEntryHeader); // intel cache line is 64byte
	constexpr uint32 stride = 1;
	uint32 ComparisonIndex = 0;
	// 차있는 entry pool에서 비교
	for (; ComparisonIndex < CurrentComparisonPoolSize; ComparisonIndex += stride)
	{
		// 헤더 비교 이후 스트링 비교
		if (ComparisonNameEntryPool[ComparisonIndex] == InEntry)
		{
			// 찾음 : 2번이상 저장하면 Cache에 저장
			if (ComparisonNameEntryCache[ComparisonCacheCursor][1] != ComparisonIndex)
			{
				ComparisonCacheCursor = (ComparisonCacheCursor + 1) % CacheSize;
				ComparisonNameEntryCache[ComparisonCacheCursor][0] = InHash;
				ComparisonNameEntryCache[ComparisonCacheCursor][1] = ComparisonIndex;
			}
			return ComparisonIndex;
		}
	}
	// 찾지못함. ComparisonIndex = CurrentPoolSize에 추가
	ComparisonNameEntryPool[ComparisonIndex] = InEntry;
	CurrentComparisonPoolSize++;
	return ComparisonIndex;
#endif
}

uint32 FNamePool::FindOrAddDisplayName(FString DisplayName)
{
	uint32 InHash = DisplayName.GetHash() % PoolSize; // pool size에 맞게 조절

	// 캐시 비교
	// cache 확인
	uint32 PossibleIdCached;
	if (FindDisplayNameCached(InHash, PossibleIdCached))
	{
		if (DisplayName == ResolveDisplay(PossibleIdCached))
		{
			return PossibleIdCached;
		}
	}

	FNameEntryHeader InHeader;
	InHeader.LowercaseProbeHash = InHash && uint16(31); // 빠른 비교를 위한 5비트 tag
	InHeader.Len = DisplayName.Len(); // 빠른 비교를 위한 길이 tag

	FNameEntry InEntry;
	InEntry.Data = DisplayName;
	InEntry.Header = InHeader;

#ifdef IS_FNAME_POOL_HASHED
	uint32 DisplayIndex = InHash;
	do
	{
		if (DisplayNameEntryPool[DisplayIndex].isEmpty()) // 빈곳을 찾음
		{
			DisplayNameEntryPool[DisplayIndex] = InEntry;
			return DisplayIndex;
		}
		if (DisplayNameEntryPool[DisplayIndex] == InEntry) //	비어있지 않음 -> 같은 값인지 확인
		{
			// 찾음 : 2번이상 저장하면 Cache에 저장
			if (DisplayNameEntryCache[DisplayCacheCursor][1] != DisplayIndex)
			{
				DisplayCacheCursor = (DisplayCacheCursor + 1) % CacheSize;
				DisplayNameEntryCache[DisplayCacheCursor][0] = InHash;
				DisplayNameEntryCache[DisplayCacheCursor][1] = DisplayIndex;
			}
			return DisplayIndex;
		}
		DisplayIndex = (DisplayIndex + 1) % PoolSize;
	} while (DisplayIndex != InHash);

	UE_LOG("FName::DisplayNameEntryPool is full.\n");
	return 0;

#else
	if (CurrentDisplayPoolSize == PoolSize)
	{
		UE_LOG("FName::DisplayNameEntryPool is full.\n");
		return 0;
	}
	constexpr uint32 stride = 1;
	uint32 DisplayIndex = 0;
	// 차있는 entry pool에서 비교
	for (; DisplayIndex < CurrentDisplayPoolSize; DisplayIndex += stride)
	{
		// 헤더 비교 이후 스트링 비교
		if(DisplayNameEntryPool[DisplayIndex] == InEntry)
		{
			// 찾음 : 2번이상 저장하면 Cache에 저장
			if (DisplayNameEntryCache[DisplayCacheCursor][1] != DisplayIndex)
			{
				DisplayCacheCursor = (DisplayCacheCursor + 1) % CacheSize;
				DisplayNameEntryCache[DisplayCacheCursor][0] = InHash;
				DisplayNameEntryCache[DisplayCacheCursor][1] = DisplayIndex;
			}
			return DisplayIndex;
		}
	}

	// 찾지못함. DisplayIndex = CurrentPoolSize에 추가
	DisplayNameEntryPool[DisplayIndex] = InEntry;
	CurrentDisplayPoolSize++;
	return DisplayIndex;
#endif
}


FString FNamePool::ResolveComparison(uint32 Id)
{
	return ComparisonNameEntryPool[Id].Data;
}

FString FNamePool::ResolveDisplay(uint32 Id)
{
	return DisplayNameEntryPool[Id].Data;
}

// 최근 Cache를 돌면서 Id를 리턴합니다.
bool FNamePool::FindComparisonNameCached(uint32 InHash, uint32& OutId)
{
	uint32 i = ComparisonCacheCursor;
	do
	{
		if (ComparisonNameEntryCache[i][0] == InHash)
		{
			OutId = ComparisonNameEntryCache[i][1];
			return true;
		}
		i = (i - 1) % CacheSize;

	} while (i != ComparisonCacheCursor);

	return false;
}

bool FNamePool::FindDisplayNameCached(uint32 InHash, uint32& OutId)
{
	uint32 i = DisplayCacheCursor;
	do
	{
		if (DisplayNameEntryCache[i][0] == InHash)
		{
			OutId = DisplayNameEntryCache[i][1];
			return true;
		}
		i = (i - 1) % CacheSize;

	} while (i != DisplayCacheCursor);

	return false;
}

const std::vector<FString> FNamePool::GetComparisonNames() const
{
	std::vector<FString> List;
#ifndef IS_FNAME_POOL_HASHED
	for (int i = 0; i < CurrentComparisonPoolSize; i++)
	{
		List.push_back(ResolveComparison(ComparisonNameEntryPool[i]));
	}
	return List;
#else
	for (int i = 0; i < PoolSize; i++)
	{
		if (!ComparisonNameEntryPool[i].isEmpty())
		{
			List.push_back(ComparisonNameEntryPool[i].Data);
		}
	}
	return List;
#endif
}

const std::vector<FString> FNamePool::GetDisplayNames() const
{
	std::vector<FString> List;
#ifndef IS_FNAME_POOL_HASHED
	for (int i = 0; i < CurrentDisplayPoolSize; i++)
	{
		List.push_back(ResolveComparison(DisplayNameEntryPool[i]));
	}
	return List;
#else
	for (int i = 0; i < PoolSize; i++)
	{
		if (!DisplayNameEntryPool[i].isEmpty())
		{
			List.push_back(DisplayNameEntryPool[i].Data);
		}
	}
	return List;
#endif
}


bool FNameEntry::operator==(const FNameEntry& Other)
{
	if (this->Header == Other.Header)
	{
		if (this->Data == Other.Data)
		{
			return true;
		}
	}
	return false;
}

bool FNameEntry::operator!=(const FNameEntry& Other)
{
	return !(*this == Other);
}

bool FNameEntryHeader::operator==(const FNameEntryHeader& Other) const
{
	return (uint16&)*this == (uint16&)Other;
}

bool FNameEntryHeader::operator==(const uint16 BitPattern) const
{
	return (uint16&)*this == (uint16&)BitPattern;
	//return *(reinterpret_cast<const uint16*>(this)) == BitPattern;
}



/////////////////////////////////
/// FName
FName::FName(const char* pStr, uint32 InNumber)
{
	FString String(pStr);
	FString StringLower = String.ToLower();
	this->DisplayIndex = FNamePool::Get().FindOrAddDisplayName(String);
	this->ComparisonIndex = FNamePool::Get().FindOrAddComparisonName(StringLower);

	this->Number = InNumber;
	IsValid = 1;
	return;
}

FName::FName(FString Str)
{
	UE_LOG("FName constructor : %s", *Str);
	size_t pos = Str.Len();
	if (pos == 0)
		return;

	// 만약 숫자가 안붙어있으면 0으로 처리
	if (!std::isdigit(Str[pos - 1]))
	{
		this->Number = 0;
	}
	else
	{
		while (pos > 0 && std::isdigit(Str[pos - 1]))
		{
			--pos;
		}
		if (pos & 0xF0000000)
		{
			UE_LOG("Max FName Number : Over 31bit.\n");
		}
		this->Number = std::stoi(*Str.SubStr(pos)) + 1; // NULL 끝에 있으려나?
	}
	
	if (pos == 0)
	{
		UE_LOG("Invalid FName.\n");
		return;
	}

	FString StringPart = Str.SubStr(0, pos);
	FString StringPartLower = StringPart.ToLower();

	this->DisplayIndex = FNamePool::Get().FindOrAddDisplayName(StringPart);
	this->ComparisonIndex = FNamePool::Get().FindOrAddComparisonName(StringPartLower);
	IsValid = 1;
	return;
}

FName::FName()
{
	DisplayIndex = 0;
	ComparisonIndex = 0; 
	Number = 0; 
}

//int32 FName::Compare(const FName& Other) const

//bool FName::operator==(const FName& Other) const


FString FName::GetString() const
{
	FString Disp = FNamePool::Get().ResolveDisplay(DisplayIndex);
	Disp += FString::FromInt(Number);
	return Disp;
}
