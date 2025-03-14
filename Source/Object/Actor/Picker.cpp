#include "Core/HAL/PlatformType.h"
#include "Core/Rendering/URenderer.h"
#include "Picker.h"

#include "Core/Input/PlayerInput.h"
#include "Object/Gizmo/GizmoHandle.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Static/FEditorManager.h"


APicker::APicker()
{    
    bIsGizmo = true;
}

FVector4 APicker::EncodeUUID(unsigned int UUID)
{
    float a = (UUID >> 24) & 0xff;
    float b = (UUID >> 16) & 0xff;
    float g = (UUID >> 8) & 0xff;
    float r = UUID & 0xff;
	
    FVector4 color = {r, g, b, a};
    
    return color;
}

int APicker::DecodeUUID(FVector4 color)
{
    return (static_cast<unsigned int>(color.W)<<24) | (static_cast<unsigned int>(color.Z)<<16) | (static_cast<unsigned int>(color.Y)<<8) | (static_cast<unsigned int>(color.X));
}

void APicker::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APicker::LateTick(float DeltaTime)
{
    AActor::LateTick(DeltaTime);

    if(APlayerInput::Get().GetMouseDown(false))
    {
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(UEngine::Get().GetWindowHandle(), &pt);


        float ratioX = UEngine::Get().GetInitializedScreenWidth() / (float)UEngine::Get().GetScreenWidth();
        float ratioY = UEngine::Get().GetInitializedScreenHeight() / (float)UEngine::Get().GetScreenHeight();
        pt.x = pt.x * ratioX;
        pt.y = pt.y * ratioY;
        
        FVector4 color = UEngine::Get().GetRenderer()->GetPixel(FVector(pt.x, pt.y, 0));

        uint32_t UUID = DecodeUUID(color);

        UActorComponent* PickedComponent = UEngine::Get().GetObjectByUUID<UActorComponent>(UUID);

        if (PickedComponent != nullptr)
        {
            AActor* PickedActor = PickedComponent->GetOwner();

            if (PickedActor == nullptr) return;
            if (PickedComponent->GetOwner()->IsGizmoActor() == false)
            {
                if (PickedActor == FEditorManager::Get().GetSelectedActor())
                {
                    FEditorManager::Get().SelectActor(nullptr);   
                }
                else
                {
                    FEditorManager::Get().SelectActor(PickedActor);
                }
            }
        }
        UE_LOG("Pick - UUID: %d", UUID);
    }

    if (APlayerInput::Get().IsPressedMouse(false))
    {
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(UEngine::Get().GetWindowHandle(), &pt);
        FVector4 color = UEngine::Get().GetRenderer()->GetPixel(FVector(pt.x, pt.y, 0));
        uint32_t UUID = DecodeUUID(color);

        UActorComponent* PickedComponent = UEngine::Get().GetObjectByUUID<UActorComponent>(UUID);\
        if (PickedComponent != nullptr)
        {
            if (AGizmoHandle* Gizmo = dynamic_cast<AGizmoHandle*>(PickedComponent->GetOwner()))
            {
                if (Gizmo->GetSelectedAxis() != ESelectedAxis::None) return;
                UCylinderComp* CylinderComp = static_cast<UCylinderComp*>(PickedComponent);
                FVector4 CompColor = CylinderComp->GetCustomColor();
                if (1.0f - FMath::Abs(CompColor.X) < KINDA_SMALL_NUMBER) // Red - X축
                {
                    Gizmo->SetSelectedAxis(ESelectedAxis::X);
                }
                else if (1.0f - FMath::Abs(CompColor.Y) < KINDA_SMALL_NUMBER) // Green - Y축
                {
                    Gizmo->SetSelectedAxis(ESelectedAxis::Y);
                }
                else  // Blue - Z축
                {
                    Gizmo->SetSelectedAxis(ESelectedAxis::Z);
                }
            }
        }
    }
    else
    {
        if (AGizmoHandle* Handle = FEditorManager::Get().GetGizmoHandle())
        {
            Handle->SetSelectedAxis(ESelectedAxis::None);
        }
    }
}

const char* APicker::GetTypeName()
{
    return "Picker";
}
