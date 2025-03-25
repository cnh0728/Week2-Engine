#include "PlayerController.h"

#include <unordered_map>
#include "Object//Actor/Camera.h"
#include "PlayerInput.h"
#include "Static/FEditorManager.h"
#include "Core/Math/Plane.h"

APlayerController::APlayerController() {

}

void APlayerController::HandleCameraMovement(float DeltaTime) {

    FVector NewVelocity(0, 0, 0);

    if (APlayerInput::Get().IsPressedMouse(true) == false)
    {
        // Camera->SetVelocity(NewVelocity);
        return;
    }

    ACamera* Camera = FEditorManager::Get().GetCamera();
    ECameraViewMode::Type cameraType = Camera->ViewMode;
    FVector MousePrePos = APlayerInput::Get().GetMousePrePos();
    FVector MousePos = APlayerInput::Get().GetMousePos();
    FVector DeltaPos = MousePos - MousePrePos;

    if (Camera->ViewMode != ECameraViewMode::Type::Perspective)
    {
        FEditorManager::Get().MoveAllOrthoCameras(cameraType, DeltaPos);
        return;
    }
    else
    {
        
    }

    FTransform CameraTransform = Camera->GetActorRelativeTransform();

    FVector TargetRotation = CameraTransform.GetRotation().GetEuler();
    TargetRotation.Y += Camera->GetCameraSensitivity() * DeltaPos.Y * DeltaTime;
    TargetRotation.Z += Camera->GetCameraSensitivity() * DeltaPos.X * DeltaTime;
    
    TargetRotation.Y = FMath::Clamp(TargetRotation.Y, -Camera->MaxYDegree, Camera->MaxYDegree);
    CameraTransform.SetRotation(TargetRotation);

    float CamSpeed = Camera->CameraSpeed;
    
    if (APlayerInput::Get().IsPressedKey(EKeyCode::A)) {
        NewVelocity -= Camera->GetRight();
    }
    if (APlayerInput::Get().IsPressedKey(EKeyCode::D)) {
        NewVelocity += Camera->GetRight();
    }
    if (APlayerInput::Get().IsPressedKey(EKeyCode::W)) {
        NewVelocity += Camera->GetForward();
    }
    if (APlayerInput::Get().IsPressedKey(EKeyCode::S)) {
        NewVelocity -= Camera->GetForward();
    }
    if (APlayerInput::Get().IsPressedKey(EKeyCode::Q))
    {
        NewVelocity -= {0.0f, 0.0f, 1.0f};
    }
    if (APlayerInput::Get().IsPressedKey(EKeyCode::E))
    {
        NewVelocity += {0.0f, 0.0f, 1.0f};
    }
    if (NewVelocity.Length() > 0.001f)
    {
        NewVelocity = NewVelocity.GetSafeNormal();
    }

    //회전이랑 마우스클릭 구현 카메라로 해야할듯?
    CameraTransform.Translate(NewVelocity * DeltaTime * CamSpeed);
    Camera->SetActorRelatvieTransform(CameraTransform); //임시용
    // FCamera::Get().SetVelocity(NewVelocity);
}

void APlayerController::HandleGizmoMovement(float DeltaTime)
{
    if (APlayerInput::Get().IsPressedMouse(false) == false)
    {
        return;
    }

    AActor* SelectedActor = FEditorManager::Get().GetSelectedActor();
    
    if (SelectedActor == nullptr) //이거를 나중엔 기즈모로 체크
    {
        return;
    }
}

void APlayerController::ProcessKeyBind(float DeltaTime)
{
    if (APlayerInput::Get().GetKeyDown(EKeyCode::Delete))
    {
        if (AActor* SelectedActor = FEditorManager::Get().GetSelectedActor())
        {
            SelectedActor->Destroy();
        }
    }
}

void APlayerController::ProcessPlayerInput(float DeltaTime) {

    HandleGizmoMovement(DeltaTime);
    HandleCameraMovement(DeltaTime);
    ProcessKeyBind(DeltaTime);
}
