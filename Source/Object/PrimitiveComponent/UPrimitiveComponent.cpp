#include "UPrimitiveComponent.h"
#include "Object/World/World.h"
#include "Object/Actor/Actor.h"
#include"Object/Cast.h"

void UPrimitiveComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPrimitiveComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}

void UPrimitiveComponent::UpdateConstantPicking(const URenderer& Renderer, const FVector4 UUIDColor)const
{
	Renderer.UpdateConstantPicking(UUIDColor);
}

void UPrimitiveComponent::UpdateConstantDepth(const URenderer& Renderer, const int Depth)const
{
	Renderer.UpdateConstantDepth(Depth);
}

void UPrimitiveComponent::Render()
{
	URenderer* Renderer = UEngine::Get().GetRenderer();
	if (Renderer == nullptr || bIsDefaultRendered == false)
	{
		return;
	}
	Renderer->RenderPrimtive(this);
}

void UPrimitiveComponent::RegisterComponentWithWorld(UWorld* World)
{
	URenderer* Renderer = UEngine::Get().GetRenderer();
	
	D3D11_PRIMITIVE_TOPOLOGY Topology = GetTopology(GetType());

	if (GetOwner()->IsBatchActor())
	{
		Renderer->AddBatchVertices(this);
	}else
	{
		if (bIsDefaultRendered)
		{
			World->AddRenderComponent(this);
		}
	}

	EPrimitiveType ComponentType = GetType();

	TArray<FVertexPNCT> Vertices = OriginVertices[ComponentType];

	TArray<uint32_t> Indices = OriginIndices[ComponentType];
	
	VertexBufferInfo BufferInfo = VertexBufferInfo(Topology, Vertices, Indices);

	Renderer->CreateVertexBuffer(GetType(), BufferInfo);
}

TSet<UPrimitiveComponent*> UPrimitiveComponent::FilterPrimitiveComponents(TSet<UActorComponent*>& Components)
{
	TSet<UPrimitiveComponent*> NewSet;

	for (auto Comp : Components)
	{
		if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Comp))
			NewSet.Add(Prim);
	}
	return NewSet;
}

void UPrimitiveComponent::SetColor(FVector4 Color)
{
	this->Color = Color;
	PixelType = ECustomColor;
}

void UPrimitiveComponent::SetTextureResource(std::string ER)
{
	this->TextureResource = ER;
	PixelType = ETexture;
}
