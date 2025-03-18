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
    bCanPick = false;
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

    APlayerInput& Input = APlayerInput::Get();

    UPrimitiveComponent* PickedComponent = nullptr;
    
    UpdateRayInfo();

    if(Input.GetMouseDown(false))
    {
        TMap<UPrimitiveComponent*, float> PickedPrimitives = PickActorByRay(); //PriorityQueue 구현
        
        float MinDistance = FLT_MAX;
        bool IsFindGizmo = false;
        for (auto& [Primitive, Distance] : PickedPrimitives)
        {
            //프리미티브 돌면서 가장 가까운 액터 찾는 포문. 기즈모면 젤 우선해서 
            if (Primitive->IsCanPick() == false)
            {
                continue;
            }
            
            AActor* ValueActor = Primitive->GetOwner();
            
            if (ValueActor->IsGizmoActor())
            {
                if (IsFindGizmo == false)
                {
                    MinDistance = Distance;
                    IsFindGizmo = true;
                    PickedComponent = Primitive;
                }else
                {
                    if (MinDistance > Distance)
                    {
                        MinDistance = Distance;
                        PickedComponent = Primitive;
                    }
                }
            }

            if (IsFindGizmo == false) //기즈모가 아니면서
            {
                if (Primitive->GetOwner()->IsCanPick()) //집을수 있는 애면서
                {
                    if (MinDistance > Distance) //더 가까우면 바꾸기
                    {
                        MinDistance = Distance;
                        PickedComponent = Primitive;
                    }
                }
            }
        }
        
        //여기서부턴 선택된 프리미티브 취소 로직
        if (PickedComponent == nullptr) 
        {
            // FEditorManager::Get().SelectActor(nullptr);   //피킹된 애가 없으면 선택없애기
            return;
        }
        
        //pickedActor가 클릭돼있을때 클릭하면 nullptr, 픽한애 없어도 nullptr
        
        if (IsFindGizmo == false) //기즈모가 아니면서
        {
            if (PickedComponent->GetOwner() == FEditorManager::Get().GetSelectedActor()) //이미 선택헀던 애면 선택 없애기
            {
                PickedComponent = nullptr;
                FEditorManager::Get().SelectPrimitive(nullptr);
                return;
            }
            //아니면 최종 선택된애로 선정
            FEditorManager::Get().SelectPrimitive(PickedComponent);   
        }else //기즈모면
        {
            AGizmoHandle* Gizmo = dynamic_cast<AGizmoHandle*>(PickedComponent->GetOwner());

            if (Gizmo == nullptr)
            {
                return;
            }
                
            UPrimitiveComponent* CylinderComp = dynamic_cast<UPrimitiveComponent*>(PickedComponent);
			FVector4 CompColor = CylinderComp->GetColor();
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
        
        if (PickedComponent != nullptr)
        {
            UE_LOG("Picked Actor: %u", PickedComponent->GetUUID());
        }
    }

    if (Input.IsPressedMouse(false))
    {
        AGizmoHandle* Gizmo = FEditorManager::Get().GetGizmoHandle();
        AActor* Actor = FEditorManager::Get().GetSelectedActor();

        if (Gizmo == nullptr || Gizmo->GetSelectedAxis() == ESelectedAxis::None || Actor == nullptr)
        {
            return;
        }
        
        float Distance = FVector::Distance(RayOrigin, Actor->GetActorRelativeTransform().GetPosition());
			
        // Ray 방향으로 Distance만큼 재계산
        FVector Result = RayOrigin + RayDir * Distance;
	
        FTransform AT = Actor->GetActorRelativeTransform();
            
        Gizmo->DoTransform(AT, Result, Actor);
    }
    else
    {
        //마우스를 놨을때
        if (AGizmoHandle* Handle = FEditorManager::Get().GetGizmoHandle())
        {
            Handle->SetSelectedAxis(ESelectedAxis::None);
        }
    }
}

void APicker::LateTick(float DeltaTime)
{
    AActor::LateTick(DeltaTime);
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

void APicker::UpdateRayInfo()
{
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(UEngine::Get().GetWindowHandle(), &pt);
    
    RECT Rect;
    GetClientRect(UEngine::Get().GetWindowHandle(), &Rect);
    int ScreenWidth = Rect.right - Rect.left;
    int ScreenHeight = Rect.bottom - Rect.top;
			 
    // 커서 위치를 NDC로 변경
    float PosX = 2.0f * pt.x / ScreenWidth - 1.0f;
    float PosY = -2.0f * pt.y / ScreenHeight + 1.0f;
			 
    // Projection 공간으로 변환
    RayOrigin = {PosX, PosY, 0.0f, 1.0f};
    RayEnd = {PosX, PosY, 1.0f, 1.0f};
			 
    // View 공간으로 변환
    FMatrix InvProjMat = UEngine::Get().GetRenderer()->GetProjectionMatrix().Inverse();
    RayOrigin = InvProjMat.TransformVector4(RayOrigin);
    RayEnd = InvProjMat.TransformVector4(RayEnd);
    RayEnd *= FEditorManager::Get().GetCamera()->GetFar();
			 
    // 마우스 포인터의 월드 위치와 방향
    FMatrix InvViewMat = FEditorManager::Get().GetCamera()->GetViewMatrix().Inverse();
    RayOrigin = InvViewMat.TransformVector4(RayOrigin);
    RayEnd = InvViewMat.TransformVector4(RayEnd);
    RayOrigin /= RayOrigin.W;
    RayEnd /= RayEnd.W;
    
    RayDir = (RayEnd - RayOrigin).GetSafeNormal();
}

TMap<UPrimitiveComponent*, float> APicker::PickActorByRay()
{
    TSet<UPrimitiveComponent*> PrimitiveComponents = GetWorld()->GetRenderComponents();
    TMap<UPrimitiveComponent*, float> PickedPrimitive; //PriorityQueue 구현
    
    for (UPrimitiveComponent* Component : PrimitiveComponents)
    {
        FVector MinBound(FLT_MAX), MaxBound(FLT_MIN);

        //정점데이터에 월드매트릭스 곱해서 x,y,z minX minY minZ maxX maxY maxZ 구하기
        FMatrix CompWorldMatrix = Component->GetComponentTransformMatrix(); //GetWorldMatrix 부모들 다 적용시키는 걸로 바꿔줘야함

        TArray<FVertexSimple> Vertices = OriginVertices[Component->GetType()];
        int Size = Vertices.Num();
        
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
            PickedPrimitive.Add(Component, Distance); //Priority Queue 구현
        }
        
    }
    return PickedPrimitive;
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

//const char* APicker::GetTypeName()
//{
//    return "Picker";
//}