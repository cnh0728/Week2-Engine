#pragma once
#include "Object/PrimitiveComponent/TextComponent.h"
#include "Core/Rendering/URenderer.h"

class UUUIDTextComponent : public UTextComponent {
	using Super = UTextComponent;

public:
	UUUIDTextComponent() = default;
	virtual ~UUUIDTextComponent() = default;
	void SetText(const FString& InText) override;
	FString GetText() const override;
	void SetTextSize(const uint32& InTextSize) override;
	uint32 GetTextSize() const override;
	void RenderText(URenderer& Renderer, const FString& InText, const FVector& InTextPos, const FVector& InTextSize) override;

private:
	uint32 UUIDTextSize = 10;
};