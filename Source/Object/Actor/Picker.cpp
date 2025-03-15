#include "Core/HAL/PlatformType.h"
#include "Core/Rendering/URenderer.h"
#include "Picker.h"

#include "Core/Input/PlayerInput.h"
#include "Object/Gizmo/GizmoHandle.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Object/World/World.h"
#include "Static/FEditorManager.h"

#include "Object/Actor/Camera.h"

APicker::APicker()
{    
    bIsGizmo = true;
}

FVector4 APicker::EncodeUUID(unsigned int UUID)
{
    float a = static_cast<float>((UUID >> 24) & 0xff);
    float b = static_cast<float>((UUID >> 16) & 0xff);
    float g = static_cast<float>((UUID >> 8) & 0xff);
    float r = static_cast<float>(UUID & 0xff);
	
    FVector4 color = {r, g, b, a};
    
    return color;
}

uint32_t APicker::DecodeUUID(FVector4 color)
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
        
        TSet<std::pair<float,AActor*>> PickedActors = PickActorByRay(FVector(pt.x, pt.y, 0.0f)); //PriorityQueue 구현
        AActor* PickedActor = nullptr;
        float MinDistance = FLT_MAX;
        for (auto& [Key, Value] : PickedActors)
        {
            if (MinDistance > Key)
            {
                MinDistance = Key;
                PickedActor = Value;
            }
        }
        
        if (PickedActors.Num() > 0)
        {
            UE_LOG("Pick %u", PickedActor->GetUUID());
        }else
        {
            UE_LOG("UnPick");
        }
        
        // PickedActor = PickActorByPixel(FVector(pt.x, pt.y, 0.0f));

        if (PickedActor == nullptr)
        {
            FEditorManager::Get().SelectActor(nullptr);   
            return;
        }
        
        //pickedActor가 클릭돼있을때 클릭하면 nullptr, 픽한애 없어도 nullptr
        
        if (PickedActor->IsGizmoActor() == false)
        {
            if (PickedActor == FEditorManager::Get().GetSelectedActor())
            {
                PickedActor = nullptr;
                FEditorManager::Get().SelectActor(nullptr);
                return;
            }
        }

        FEditorManager::Get().SelectActor(PickedActor);   
        
        if (PickedActor != nullptr)
        {
            UE_LOG("Picked Actor: %u", PickedActor->GetUUID());
        }
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

bool APicker::IntersectsRay(const FVector& rayOrigin, const FVector& rayDir, float& Distance, FVector MinBound, FVector MaxBound)
{
    //MinBound 와 MaxBound에 Offset을 더해주거나 빼면 그만큼 더 넓거나 좁은 범위의 큐브를 만든다 (Min에는 Offset을 빼고 Max에는 Offset을 더하기)
    double tMin = -FLT_MAX;
    double tMax = FLT_MAX;
    const double epsilon = FLT_EPSILON;
    if (fabs(rayDir.X) < epsilon) {
        if (rayOrigin.X < MinBound.X || rayOrigin.X > MaxBound.X)
            return false;
    }
    else {
        double t1 = (MinBound.X - rayOrigin.X) / rayDir.X;
        double t2 = (MaxBound.X - rayOrigin.X) / rayDir.X;
        if (t1 > t2)
            std::swap(t1, t2);
        tMin = (tMin < t1) ? t1 : tMin;
        tMax = (tMax < t2) ? tMax : t2;
    }
    
    if (fabs(rayDir.Y) < epsilon) {
        if (rayOrigin.Y < MinBound.Y || rayOrigin.Y > MaxBound.Y)
            return false;
    }
    else {
        double t1 = (MinBound.Y - rayOrigin.Y) / rayDir.Y;
        double t2 = (MaxBound.Y - rayOrigin.Y) / rayDir.Y;
        if (t1 > t2) std::swap(t1, t2);
        tMin = (tMin < t1) ? t1 : tMin;
        tMax = (tMax < t2) ? tMax : t2;
    }
    if (fabs(rayDir.Z) < epsilon) {
        if (rayOrigin.Z < MinBound.Z || rayOrigin.Z > MaxBound.Z)
            return false;
    }
    else
    {
        double t1 = (MinBound.Z - rayOrigin.Z) / rayDir.Z;
        double t2 = (MaxBound.Z - rayOrigin.Z) / rayDir.Z;
        if (t1 > t2) std::swap(t1, t2);
        tMin = (tMin < t1) ? t1 : tMin;
        tMax = (tMax < t2) ? tMax : t2;
    }
    if (tMax >= tMin && tMax >= 0) {
        Distance = tMin;
        return true;
    }
    return false;
}

TSet<std::pair<float, AActor*>> APicker::PickActorByRay(FVector MousePos)
{
    RECT Rect;
    GetClientRect(UEngine::Get().GetWindowHandle(), &Rect);
    int ScreenWidth = Rect.right - Rect.left;
    int ScreenHeight = Rect.bottom - Rect.top;
			 
    // 커서 위치를 NDC로 변경
    float PosX = 2.0f * MousePos.X / ScreenWidth - 1.0f;
    float PosY = -2.0f * MousePos.Y / ScreenHeight + 1.0f;
			 
    // Projection 공간으로 변환
    FVector4 RayOrigin {PosX, PosY, 0.0f, 1.0f};
    FVector4 RayEnd {PosX, PosY, 1.0f, 1.0f};
			 
    // View 공간으로 변환
    FMatrix InvProjMat = UEngine::Get().GetRenderer()->GetProjectionMatrix().Inverse();
    RayOrigin = InvProjMat.TransformVector4(RayOrigin);
    RayOrigin.W = 1;
    RayEnd = InvProjMat.TransformVector4(RayEnd);
    RayEnd *= 1000.0f;  // 프러스텀의 Far 값이 적용이 안돼서 수동으로 곱함
    RayEnd.W = 1;
			 
    // 마우스 포인터의 월드 위치와 방향
    FMatrix InvViewMat = FEditorManager::Get().GetCamera()->GetViewMatrix().Inverse();
    RayOrigin = InvViewMat.TransformVector4(RayOrigin);
    RayOrigin /= RayOrigin.W = 1;
    RayEnd = InvViewMat.TransformVector4(RayEnd);
    RayEnd /= RayEnd.W = 1;
    TSet<UPrimitiveComponent*> PrimitiveComponents = GetWorld()->GetRenderComponents();
    TSet<std::pair<float,AActor*>> PickedActors; //PriorityQueue 구현

    FVector RayDir = (RayEnd - RayOrigin).GetSafeNormal();
    
    for (UPrimitiveComponent* Component : PrimitiveComponents)
    {
        FVector MinBound(FLT_MAX), MaxBound(FLT_MIN);

        //정점데이터에 월드매트릭스 곱해서 x,y,z minX minY minZ maxX maxY maxZ 구하기
        FMatrix CompWorldMatrix = Component->GetComponentTransformMatrix(); //GetWorldMatrix 부모들 다 적용시키는 걸로 바꿔줘야함
        //이제 정점 돌면서 월드매트릭스 곱하기
        BufferInfo Info = UEngine::Get().GetRenderer()->GetBufferInfo(Component->GetType());
        FVertexSimple* Vertices = Info.GetVertices();
        int Size = Info.GetSize();
        
        for (int i=0; i<Size; i++)
        {
            FVector4 CompVertex = FVector4(Vertices[i].X, Vertices[i].Y, Vertices[i].Z, 1.0f);
            FVector4 WorldVertexLocation = CompVertex * CompWorldMatrix;
            
            //최소값과 최대값 구하기
            MinBound.X = min(MinBound.X, WorldVertexLocation.X);
            MinBound.Y = min(MinBound.Y, WorldVertexLocation.Y);
            MinBound.Z = min(MinBound.Z, WorldVertexLocation.Z);
            MaxBound.X = max(MaxBound.X, WorldVertexLocation.X);
            MaxBound.Y = max(MaxBound.Y, WorldVertexLocation.Y);
            MaxBound.Z = max(MaxBound.Z, WorldVertexLocation.Z);
        }

        //Priority Queue 구현해서 Add하기
        float Distance = FVector::Distance(RayOrigin, Component->GetComponentTransform().GetPosition());
        if (IntersectsRay(RayOrigin, RayDir, Distance, MinBound, MaxBound))
        {
            PickedActors.Add({Distance,Component->GetOwner()}); //Priority Queue 구현
        }
        
    }
    return PickedActors;
}

AActor* APicker::PickActorByPixel(FVector MousePos)
{
    // wireframe모드면 작동안함
    // 액터와의 거리
        
    FVector4 color = UEngine::Get().GetRenderer()->GetPixel(FVector(MousePos.X, MousePos.Y, 0));
        
    uint32_t UUID = DecodeUUID(color);
        
    UActorComponent* PickedComponent = UEngine::Get().GetObjectByUUID<UActorComponent>(UUID);

    AActor* PickedActor = nullptr;

    if (PickedComponent != nullptr)
    {
        PickedActor = PickedComponent->GetOwner();
    }

    // UE_LOG("Pick - UUID: %d", UUID);

    return PickedActor;
}

const char* APicker::GetTypeName()
{
    return "Picker";
}