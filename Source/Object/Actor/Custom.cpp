#include "Custom.h"

#include "Object/PrimitiveComponent/CustomComponent.h"
#include <cstdlib>
#include <ctime>    




ACustom::ACustom()
{
    bCanEverTick = true;

    UCustomComponent* CustomComp = AddComponent<UCustomComponent>();
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
    if (UCustomComponent* CustomComp = GetComponentByClass<UCustomComponent>())
    {
        CustomComp->LoadFromObj(InPath);
    }
}
