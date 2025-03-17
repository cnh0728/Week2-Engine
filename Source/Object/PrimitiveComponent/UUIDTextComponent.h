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
	void SetUUIDTextSize(const uint32& InTextSize) {
		UUIDTextSize = InTextSize;
	}
	uint32 GetUUIDTextSize() const {
		return UUIDTextSize;
	}

	void RenderText(URenderer& Renderer, const FString& InText, const FVector& InTextPos, const FVector& InTextSize) override;

private:
	uint32 UUIDTextSize = 10;
};