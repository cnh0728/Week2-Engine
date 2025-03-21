#include "Texture.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

ATexture::ATexture()
{
    bCanEverTick = true;
    
    UTextureComp* TextureComp = AddComponent<UTextureComp>();
    TextureComp->SetupAttachment(RootComponent);
    TextureComp->SetTextureResource(ETextureResource::cat);
}

void ATexture::BeginPlay()
{
    SetUUIDTag();

    Super::BeginPlay();
}

void ATexture::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}