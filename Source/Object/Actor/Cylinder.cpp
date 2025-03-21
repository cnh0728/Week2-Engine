#include "Cylinder.h"
#include <Object/PrimitiveComponent/UPrimitiveComponent.h>

ACylinder::ACylinder()
{
    bCanEverTick = true;

    UCylinderComp* CylinderComponent = AddComponent<UCylinderComp>();
    RootComponent = CylinderComponent;
	
    SetActorRelativeTransform(FTransform());

}

void ACylinder::BeginPlay()
{
    SetUUIDTag();
    
    Super::BeginPlay();
}

void ACylinder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

//const char* ACylinder::GetTypeName()
//{
//    return "Cylinder";
//}
