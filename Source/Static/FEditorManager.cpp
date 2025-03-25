#include "FEditorManager.h"
#include "Core/Engine.h"
#include "Object/World/World.h"
#include "Object/Gizmo/GizmoHandle.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Transform.h"

void FEditorManager::SelectComponent(UPrimitiveComponent* NewComponent)
{
    if (GizmoHandle == nullptr) //기즈모 띄우는거면 생성
    {
		GizmoHandle = UEngine::Get().GetWorld()->SpawnActor<AGizmoHandle>();
        GizmoHandle->SetActive(false);
    }

	if (NewComponent == nullptr || NewComponent == SelectedComponent) //이미 선택헀던 애면 선택 없애기
	{
		ReleasePick();
		return;
	}

	AActor* NewActor = NewComponent->GetOwner();

	switch (CurrentPickState)
	{
		case PickState::None:
			SetPickState(PickState::Actor);
			break;
		case PickState::Actor:
			GizmoHandle->SetActive(true);
			if (SelectedActor == NewActor)
			{
				SetPickState(PickState::Component);
			}
			break;
		case PickState::Component:
			if (SelectedComponent == NewComponent)
			{
				SetPickState(PickState::None);
			}
			else if (SelectedActor != NewActor)
			{
				SetPickState(PickState::Actor);
			}
			break;
		default:
			break;
	}

	switch (CurrentPickState)
	{
		case PickState::None:
			ReleasePick();
			return;
		case PickState::Actor:
			GizmoHandle->SetActive(true);
			SelectedActor = NewActor;
			NewActor->Pick();

			if (SelectedComponent)
			{
				SelectedComponent->Pick(false);
				SelectedComponent = nullptr;
			}
			break;
		case PickState::Component:
			GizmoHandle->SetActive(true);
			SelectedComponent = NewComponent;
			NewComponent->Pick(true);
			break;
	default:
		break;
	}
}

void FEditorManager::ReleasePick()
{
	SetPickState(PickState::None);
	
	if (SelectedActor)
	{
		SelectedActor->UnPick();
		SelectedActor = nullptr;
	}
	if (SelectedComponent)
	{
		SelectedComponent->Pick(false);
		SelectedComponent = nullptr;
	}
	if (GizmoHandle)
	{
		GizmoHandle->SetActive(false);
	}
}

void FEditorManager::SelectActor(AActor* NewActor)
{
	if (GizmoHandle == nullptr) //기즈모 띄우는거면 생성
	{
		GizmoHandle = UEngine::Get().GetWorld()->SpawnActor<AGizmoHandle>();
		GizmoHandle->SetActive(false);
	}

	if (NewActor == nullptr || NewActor == SelectedActor) //이미 선택헀던 애면 선택 없애기
	{
		ReleasePick();
		return;
	}
	
	GizmoHandle->SetActive(true);
	SelectedActor = NewActor;
	SelectedActor->Pick();
}

void FEditorManager::SetCamera(ACamera* NewCamera)
{
    Camera = NewCamera;
}
