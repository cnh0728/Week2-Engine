#include "UUIDTextComponent.h"

void UUUIDTextComponent::SetText(const FString& InText) {
	Text = "UUID: " + InText;
}

FString UUUIDTextComponent::GetText() const {
	return Super::GetText();
}

void UUUIDTextComponent::SetTextSize(const uint32& InTextSize)
{
	UUIDTextSize = InTextSize;
}

uint32 UUUIDTextComponent::GetTextSize() const
{
	return UUIDTextSize;
}

void UUUIDTextComponent::RenderText(URenderer& Renderer, const FString& InText, const FVector& InTextPos, const FVector& InTextSize)
{
	Renderer.RenderText(InText, InTextPos, InTextSize);
}
