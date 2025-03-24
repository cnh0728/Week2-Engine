#include "Custom.h"

#include "Object/PrimitiveComponent/CustomComponent.h"
#include <cstdlib>
#include <ctime>    




ACustom::ACustom()
{
    bCanEverTick = true;

    UCustomComponent* CustomComp = AddComponent<UCustomComponent>();
    CustomComp->SetupAttachment(RootComponent);
    srand(static_cast<unsigned int>(time(nullptr))); 
    const char* modelPath = (rand() % 2 == 0) ? "Models/table.obj" : "Models/pirate.obj";
    //CustomComp->LoadFromObj(modelPath);
    CustomComp->LoadFromObj("pirate2");
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