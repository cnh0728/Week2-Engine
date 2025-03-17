#pragma once
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

class URenderer;
class UText;

class UTextComponent : public UPrimitiveComponent {
	using Super = UPrimitiveComponent;

public:
	UTextComponent();
	virtual ~UTextComponent() = default;
	virtual void SetText(const FString& InText);
	virtual FString GetText() const;
	virtual void RenderText(URenderer& Renderer, const FString& InText, const FVector& InTextPos, const FVector& InTextSize);
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Text;
	}

protected:
	FString Text;
	UText* TextRenderClass;
};
