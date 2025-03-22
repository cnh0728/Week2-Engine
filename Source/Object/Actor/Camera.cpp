#include "Camera.h"

#include "Core/Rendering/URenderer.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"


ACamera::ACamera()
{
    DontDestroy();
    bCanPick = false;
    
    Near = 0.1f;
    Far = 1000.f;
    FieldOfView = 45.f;
    ProjectionMode = ECameraProjectionMode::Perspective;
    
    SetActorRelatvieTransform(GetSpawnTransform());
}

void ACamera::SetFieldOfView(float Fov)
{
    FieldOfView = Fov;
}

void ACamera::SetFar(float Far)
{
    this->Far = Far;
}

void ACamera::SetNear(float Near)
{
    this->Near = Near;
}

void ACamera::SetCameraSensitivity(float sensitivity)
{
    float clampedSensitivity = FMath::Clamp(sensitivity, 0.1f, 10.0f);
    this->CameraSensitivity = clampedSensitivity;
}

void ACamera::SetCameraViewMode(ECameraViewMode::Type cameraViewMode)
{
    FVector Position = GetActorRelativeTransform().GetPosition();
    FQuat Rotation;
    switch (cameraViewMode)
    {
    case ECameraViewMode::Type::Front:
        Rotation = FQuat::AxisAngleToQuaternion(FVector(0, 0, 1), 180);
        break;
    case ECameraViewMode::Type::Back:
        Rotation = FQuat::AxisAngleToQuaternion(FVector(0, 0, 1), 0);
        break;
    case ECameraViewMode::Type::Top:
        Rotation = FQuat::AxisAngleToQuaternion(FVector(0, 1, 0), 90);
        break;
    case ECameraViewMode::Type::Bottom:
        Rotation = FQuat::AxisAngleToQuaternion(FVector(0, 1, 0), -90);
        break;
    case ECameraViewMode::Type::Left:
        Rotation = FQuat::AxisAngleToQuaternion(FVector(0, 0, 1), 90);
        break;
    case ECameraViewMode::Type::Right:
        Rotation = FQuat::AxisAngleToQuaternion(FVector(0, 0, 1), -90);
        break;
    default:
        break;
    }        
    FTransform frontTransform = GetActorRelativeTransform();
    frontTransform.SetRotation(Rotation);
    SetActorRelatvieTransform(frontTransform);
}

float ACamera::GetFieldOfView() const
{
    return  FieldOfView;
}

float ACamera::GetNear() const
{
    return Near;
}

float ACamera::GetFar() const
{
    return Far;
}

float ACamera::GetCameraSensitivity() const
{
    return CameraSensitivity;
}
