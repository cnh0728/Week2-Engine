#pragma once
#include <iostream>
#include "Core/EngineTypes.h"

enum class EEngineShowFlags : uint16
{
	SF_Primitives = 1 << 0,
	SF_BillboardText = 1 << 1,
};

class FEngineShowFlags {
public:
	FEngineShowFlags() : Flags(static_cast<uint16_t>(EEngineShowFlags::SF_Primitives) |
		static_cast<uint64_t>(EEngineShowFlags::SF_BillboardText)) {
	}

	bool IsSet(EEngineShowFlags Flag) const {
		return (Flags & static_cast<uint16_t>(Flag)) != 0;
	}

	void Set(EEngineShowFlags Flag, bool bEnabled)
	{
		if (bEnabled)
			Flags |= static_cast<uint16_t>(Flag);
		else
			Flags &= ~static_cast<uint16_t>(Flag);
	}

	void Toggle(EEngineShowFlags Flag) {
		Flags ^= static_cast<uint16_t>(Flag);
	}

	void Print() const {
		UE_LOG(TEXT("SF Primitives : %d, SF BillboardText : %d"),
			IsSet(EEngineShowFlags::SF_Primitives),
			IsSet(EEngineShowFlags::SF_BillboardText));
	}
private:
	// flag를 비트 단위로 저장 & bitwise 연산으로 각각의 flag 확인 및 수정
	uint16_t Flags;
};