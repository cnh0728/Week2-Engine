#include "TextComponent.h"

UTextComponent::UTextComponent()
{
	Text = FString();
}

void UTextComponent::SetText(const FString& InText)
{
	Text = InText;
}

FString UTextComponent::GetText() const
{
	return Text;
}

void UTextComponent::RenderText(URenderer& Renderer, const FString& InText, const FVector& InTextPos, const FVector& InTextSize)
{
	Renderer.RenderText(InText, InTextPos, InTextSize);
}
