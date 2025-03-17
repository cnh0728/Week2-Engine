#include "UUIDTextComponent.h"

void UUUIDTextComponent::SetText(const FString& InText) {
	Text = "UUID: " + InText;
}

FString UUUIDTextComponent::GetText() const {
	return Super::GetText();
}

void UUUIDTextComponent::RenderText(URenderer& Renderer, const FString& InText, const FVector& InTextPos, uint32 InTextSize)
{
	Renderer.RenderText(InText, InTextPos, InTextSize);
}
