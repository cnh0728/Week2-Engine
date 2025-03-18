#pragma once
#include <iostream>
#include "Core/EngineTypes.h"

enum class EEngineShowFlags : uint64
{
	SF_Primitives = 1 << 0,
	SF_BillboardText = 1 << 1,
};

class FEngineShowFlags {
public:
	FEngineShowFlags() : Flags(static_cast<uint64_t>(EEngineShowFlags::SF_Primitives) |
							   static_cast<uint64_t>(EEngineShowFlags::SF_BillboardText)) {}

	bool IsSet(EEngineShowFlags Flag) const {
		return (Flags & static_cast<uint64_t>(Flag)) != 0;
	}

	void Set(EEngineShowFlags Flag, bool bEnabled)
	{
		if (bEnabled)
			Flags |= static_cast<uint64_t>(Flag);
		else
			Flags &= ~static_cast<uint64_t>(Flag);
	}

	void Toggle(EEngineShowFlags Flag) {
		Flags ^= static_cast<uint64_t>(Flag);
	}

	void Print() const {
		UE_LOG(TEXT("SF Primitives : %d, SF BillboardText : %d"),
			IsSet(EEngineShowFlags::SF_Primitives),
			IsSet(EEngineShowFlags::SF_BillboardText));
	}
private:
	// 각 플래그의 on/off상태를 Flags의 각 비트에 저장
	uint64_t Flags;
};