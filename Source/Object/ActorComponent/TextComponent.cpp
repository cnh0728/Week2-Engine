#include "TextComponent.h"
#include "Core/Rendering/Text/Text.h"
#include "Core/Rendering/URenderer.h"

UTextComponent::UTextComponent()
{
	Text = FString();
	bCanBeRendered = true;
}

void UTextComponent::SetText(const FString& InText)
{
	Text = InText;
}

const FString& UTextComponent::GetText() const
{
	return Text;
}

void UTextComponent::RenderText(URenderer& Renderer, const FString& InText)
{
	Renderer.RenderText(InText, GetRelativeTransform().GetPosition());
}
