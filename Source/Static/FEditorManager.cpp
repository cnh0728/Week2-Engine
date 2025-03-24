#include "FEditorManager.h"
#include "Core/Engine.h"
#include "Object/World/World.h"
#include "Object/Gizmo/GizmoHandle.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Transform.h"
#include "Object/Actor/Camera.h"

void FEditorManager::SelectPrimitive(UPrimitiveComponent* NewPrimitive)
{
    if (GizmoHandle == nullptr)
    {
		GizmoHandle = UEngine::Get().GetWorld()->SpawnActor<AGizmoHandle>();
    	GizmoHandle->SetDepth(1);
        GizmoHandle->SetActive(false);
    }

	//NewPritmivie가 null이면 null로 다 바꿔줘야하고
	//null이 아니면 정상로직
	
	if (NewPrimitive == nullptr)
	{
		SelectedActor->UnPick();
        
		GizmoHandle->SetActive(false);

		SelectedActor = nullptr;
		SelectedComponent = nullptr;
		return;
	}
	
	AActor* NewActor = NewPrimitive->GetOwner();
	
	if (SelectedActor == NewActor) //같은애 선택하면 선택 취소
		return;
	
    if (SelectedActor != nullptr && SelectedActor != NewActor) //액터가 있는데, 뉴액터랑 다른애면 선택취소 맞지 ㅇㅇ
    {
        SelectedActor->UnPick();
        
        GizmoHandle->SetActive(false);
    }

	SelectedActor = NewActor;
	SelectedComponent = NewPrimitive;
	
    if (SelectedActor != nullptr)
    {
        SelectedActor->Pick();
        GizmoHandle->SetActive(true);
	}
}

void FEditorManager::SelectActor(AActor* NewActor)
{
	if (GizmoHandle == nullptr)
	{
		GizmoHandle = UEngine::Get().GetWorld()->SpawnActor<AGizmoHandle>();
		GizmoHandle->SetDepth(1);
		GizmoHandle->SetActive(false);
	}

	if (SelectedActor == NewActor)
		return;
	
	if (SelectedActor != nullptr && SelectedActor != NewActor)
	{
		SelectedActor->UnPick();
        
		GizmoHandle->SetActive(false);
	}

	SelectedActor = NewActor;
	SelectedComponent = nullptr;
	
	if (SelectedActor != nullptr)
	{
		SelectedActor->Pick();
		GizmoHandle->SetActive(true);
		//FVector Pos = SelectedActor->GetActorTransform().GetPosition();
		//FTransform GizmoTransform = GizmoHandle->GetActorTransform();
		//GizmoTransform.SetPosition(Pos);
		//GizmoHandle->SetActorTransform(GizmoTransform);
		//GizmoHandle
	}
}

void FEditorManager::SetCamera(ACamera* NewCamera)
{
    Camera = NewCamera;
}

void FEditorManager::SetOrthoCamera(ECameraViewMode::Type type)
{
	Camera = OrthogonalCamera[type];
}

void FEditorManager::AddOrthoCamera(ECameraViewMode::Type type, ACamera* NewMultiCamera)
{
	OrthogonalCamera.Add(type, NewMultiCamera);
}

void FEditorManager::MoveAllOrthoCameras(ECameraViewMode::Type cameraType, FVector MouseDeltaPos)
{
	if (cameraType == ECameraViewMode::Type::Perspective)
	{
		return;
	}
	FVector deltaMove(0,0,0);
	float x = MouseDeltaPos.X*0.01f;
	float y = MouseDeltaPos.Y*0.01f;
	switch (cameraType)
	{
	case ECameraViewMode::Type::Front:
		deltaMove.Z += y;
		deltaMove.Y += -x;
		break;
	case ECameraViewMode::Type::Back:
		deltaMove.Z += y;
		deltaMove.Y += x;
		break;
	case ECameraViewMode::Type::Top:
		deltaMove.X += y;
		deltaMove.Y += x;
		break;
	case ECameraViewMode::Type::Bottom:
		deltaMove.X += -y;
		deltaMove.Y += x;
		break;
	case ECameraViewMode::Type::Left:
		deltaMove.Z += y;
		deltaMove.X += -x;
		break;
	case ECameraViewMode::Type::Right:
		deltaMove.Z += y;
		deltaMove.X += x;
		break;
	default:
		break;
	}

	for (int i = static_cast<int>(ECameraViewMode::Type::Front); 
		i < static_cast<int>(ECameraViewMode::Type::Perspective); ++i)
	{
		ECameraViewMode::Type mode = static_cast<ECameraViewMode::Type>(i);
		MoveOrthoCamera(mode, deltaMove);
	}

}

void FEditorManager::MoveOrthoCamera(ECameraViewMode::Type cameraType, FVector deltaMove)
{
	ACamera* camera = OrthogonalCamera[cameraType];
	FTransform cameraTransform = camera->GetActorRelativeTransform();
	cameraTransform.Translate(deltaMove);
	camera->SetActorRelatvieTransform(cameraTransform);

}
