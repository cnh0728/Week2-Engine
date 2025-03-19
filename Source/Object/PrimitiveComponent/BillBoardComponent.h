#pragma once
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

class UBillBoardComponent : public UPrimitiveComponent {
	using Super = UPrimitiveComponent;

public:
	UBillBoardComponent();
	virtual ~UBillBoardComponent() = default;
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Texture;
	}
};