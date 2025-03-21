#pragma once
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Object/ObjectMacros.h"
class UBillBoardComponent : public UPrimitiveComponent {
	using Super = UPrimitiveComponent;
	DECLARE_CLASS(UBillBoardComponent, UPrimitiveComponent)
public:
	UBillBoardComponent();
	virtual ~UBillBoardComponent() = default;
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Texture;
	}
};