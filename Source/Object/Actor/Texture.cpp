#include "Texture.h"

#include "Object/PrimitiveComponent/TextureComponent.h"

ATexture::ATexture()
{
    bCanEverTick = true;
    
    UTextureComponent* TextureComp = AddComponent<UTextureComponent>();
    TextureComp->SetupAttachment(RootComponent);
    TextureComp->SetTextureResource("Textures/cat.png");
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