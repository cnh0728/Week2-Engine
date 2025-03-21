#pragma once
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Object/ObjectMacros.h"
class URenderer;
class UText;

class UTextComponent : public UPrimitiveComponent {
	using Super = UPrimitiveComponent;
	DECLARE_CLASS(UTextComponent, UPrimitiveComponent)
public:
	UTextComponent();
	virtual ~UTextComponent() = default;

	virtual FMatrix GetComponentTransformMatrix() override;
	virtual void SetText(const FString& InText);
	virtual FString GetText() const;
	virtual void RenderText(URenderer& Renderer, const FString& InText, const FVector& InTextPos, const FVector& InTextSize);
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Texture;
	}
	virtual void SetTextSize(const uint32& InTextSize);
	virtual uint32 GetTextSize() const;
	bool bIsUUIDText = false;

protected:
	FString Text;
	uint32 TextSize = 10;
	UText* TextRenderClass;
};
