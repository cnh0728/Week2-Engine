#include "TextComponent.h"
#include "Static/FEditorManager.h"
#include "Object/Actor/Camera.h"

UTextComponent::UTextComponent()
{
	bCanPick = false;
	Text = FString();
}

FMatrix UTextComponent::GetComponentTransformMatrix()
{
	if (!bIsUUIDText) return Super::GetComponentTransformMatrix();

	// UUID	Text일 경우 우상단에 표시
	ACamera* Cam = FEditorManager::Get().GetCamera();
	FVector TargetPos = this->GetAttachmentRoot()->GetRelativeTransform().GetPosition();
	FVector TargetTopRight = TargetPos + (Cam->GetRight() + Cam->GetUp()) * 0.5;
	return FMatrix(FVector4(0, 0, 0, 0), FVector4(0, 0, 0, 0), FVector4(0, 0, 0, 0), FVector4(TargetTopRight, 1.0f));

	//FVector CamPos = ->GetActorRelativeTransform().GetPosition();
	//FVector WorldUp(0, 0, 1.0); // 텍스트의 왼쪽 위는 지정된 위치 -> 지정된 위치 오른쪽 아래에 그려짐

	//FVector CamToTargetRight = FVector::CrossProduct(CamPos - TargetPos, ).Normalize();
	//FVector CamToTargetUp = FVector::CrossProduct(CamPos - TargetPos, CamToTargetRight).Normalize();
	//CamToTargetRight.Normalize();

	//FVector TargetTopRight = TargetPos + (CamToTargetRight) * 0.5;
	////FVector TargetTopRight = TargetPos + (CamToTargetRight + CamToTargetUp) * 0.5;
	//return FMatrix(FVector4(0,0,0,0), FVector4(0, 0, 0, 0), FVector4(0, 0, 0, 0), FVector4(TargetTopRight, 1.0f));
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

void UTextComponent::SetTextSize(const uint32& InTextSize)
{
	TextSize = InTextSize;
}

uint32 UTextComponent::GetTextSize() const
{
	return TextSize;
}
