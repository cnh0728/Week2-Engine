#include "Custom.h"

#include "Object/PrimitiveComponent/StaticMeshComponent.h"
#include <cstdlib>
#include <ctime>    




ACustom::ACustom()
{
    bCanEverTick = true;

    UStaticMeshComponent* CustomComp = AddComponent<UStaticMeshComponent>();
    CustomComp->SetupAttachment(RootComponent);
    //srand(static_cast<unsigned int>(time(nullptr))); 
    // char* modelPath = (rand() % 2 == 0) ? "Models/table.obj" : "Models/pirate.obj";
    //CustomComp->LoadFromObj(modelPath);
    CustomComp->LoadFromObj("12140_Skull_v3");
}

void ACustom::BeginPlay()
{
    SetUUIDTag();

    Super::BeginPlay();
}

void ACustom::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACustom::SetObjPath(const std::string& InPath)
{
    if (UStaticMeshComponent* CustomComp = GetComponentByClass<UStaticMeshComponent>())
    {
        CustomComp->LoadFromObj(InPath);
    }
}
