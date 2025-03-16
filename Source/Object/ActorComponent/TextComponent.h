#pragma once
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

class URenderer;
class UText;

class UTextComponent : public UPrimitiveComponent {
	using Super = UPrimitiveComponent;
public:
	UTextComponent();
	virtual ~UTextComponent() = default;
	void SetText(const FString& InText);
	const FString& GetText() const;
	void RenderText(URenderer& Renderer, const FString& InText);
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Text;
	}
protected:
	FString Text;
	UText* TextRenderClass;
};

class UUUIDTextComponent : public UTextComponent {
	using Super = UTextComponent;

public:
	UUUIDTextComponent() = default;
	virtual ~UUUIDTextComponent() = default;
	void SetUUIDText(uint32 UUID) {
		Text = "UUID: " + FString::FromInt(UUID);
	}
};