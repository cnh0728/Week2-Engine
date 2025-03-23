#pragma once

#include "Core/Engine.h"
#include "Object/USceneComponent.h"
#include "Primitive/PrimitiveVertices.h"
#include "Core/Math/Plane.h"
#include "Object/ObjectMacros.h"

class UPrimitiveComponent : public USceneComponent
{
	using Super = USceneComponent;
	DECLARE_CLASS(UPrimitiveComponent, USceneComponent)
public:
	UPrimitiveComponent() = default;
	virtual ~UPrimitiveComponent() = default;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void UpdateConstantPicking(const URenderer& Renderer, FVector4 UUIDColor) const;
	void UpdateConstantDepth(const URenderer& Renderer, int Depth) const;
	virtual void Render();

	virtual EPrimitiveType GetType() { return EPrimitiveType::EPT_None; }

	static D3D11_PRIMITIVE_TOPOLOGY GetTopology(EPrimitiveType Type) {
		switch (Type)
		{
		case EPrimitiveType::EPT_Line:
		case EPrimitiveType::EPT_BoundingBox:
		case EPrimitiveType::EPT_Spotlight:
			return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		default:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}
	}

public:
	virtual void RegisterComponentWithWorld(class UWorld* World);

public:
	void SetIsDefaultRendered(bool bRender) { bIsDefaultRendered = bRender; }
	bool GetIsDefaultRendered() const { return bIsDefaultRendered; }
	
	void SetIsOrthoGraphic(bool IsOrtho) { bIsOrthoGraphic = IsOrtho; }
	bool GetIsOrthoGraphic() { return bIsOrthoGraphic;}
	bool IsCanPick() const { return bCanPick; }
	void SetCanPick(bool bValue) { bCanPick = bValue; }
protected:
	bool bCanPick = false;
	bool bIsDefaultRendered = false;
	bool bIsOrthoGraphic = false;
	FVector4 CustomColor = FVector4(1.0f, 1.0f, 1.0f, 1.0f);

public:
	// UScenecomponent중에 PrimitiveComponent만 추출
	static TSet<UPrimitiveComponent*> FilterPrimitiveComponents(TSet<UActorComponent*>& Components);
	
	void SetColor(FVector4 Color);
	FVector4 GetColor() const { return Color; }

	void SetTextureResource(std::string ER);
	std::string GetTextureResource() const { return TextureResource; }

	EPixelType GetPixelType() const { return PixelType; }
	void SetPixelType(EPixelType type) { PixelType = type; }

private:
	FVector4 Color = FVector4::One();
	EPixelType PixelType = EDefalutColor;
	std::string TextureResource = "Textures/cat.png";
};

class UCubeComp : public UPrimitiveComponent
{
	using Super = UPrimitiveComponent;
	DECLARE_CLASS(UCubeComp, UPrimitiveComponent)
public:
	UCubeComp()
	{
		bIsDefaultRendered = true;
		bCanPick = true;
	}
	virtual ~UCubeComp() = default;
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Cube;
	}
};

class USphereComp : public UPrimitiveComponent
{
	using Super = UPrimitiveComponent;
	DECLARE_CLASS(USphereComp, UPrimitiveComponent)
public:
	USphereComp()
	{
		bIsDefaultRendered = true;
		bCanPick = true;
	}
	virtual ~USphereComp() = default;
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Sphere;
	}
};

class UTriangleComp : public UPrimitiveComponent
{
	using Super = UPrimitiveComponent;
	DECLARE_CLASS(UTriangleComp, UPrimitiveComponent)
public:
	UTriangleComp()
	{
		bIsDefaultRendered = true;
		bCanPick = true;

	}
	virtual ~UTriangleComp() = default;
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Triangle;
	}
};

class ULineComp : public UPrimitiveComponent
{
	using Super = UPrimitiveComponent;
	DECLARE_CLASS(ULineComp, UPrimitiveComponent)

public:
	ULineComp()
	{
		bIsDefaultRendered = true;
		bCanPick = false;
	}
	virtual ~ULineComp() = default;
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Line;
	}
};

class UCylinderComp : public UPrimitiveComponent
{
	using Super = UPrimitiveComponent;
	DECLARE_CLASS(UCylinderComp, UPrimitiveComponent)

public:
	UCylinderComp()
	{
		bIsDefaultRendered = true;
		bCanPick = true;

	}
	virtual ~UCylinderComp() = default;
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Cylinder;
	}
};

class UConeComp : public UPrimitiveComponent
{
	using Super = UPrimitiveComponent;
	DECLARE_CLASS(UConeComp, UPrimitiveComponent)
public:
	UConeComp()
	{
		bIsDefaultRendered = true;
		bCanPick = true;
	}
	virtual ~UConeComp() = default;
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Cone;
	}
};

class USpotlightComp : public UPrimitiveComponent
{
	using Super = UPrimitiveComponent;
	DECLARE_CLASS(USpotlightComp, UPrimitiveComponent)
public:
	USpotlightComp()
	{
		bIsDefaultRendered = true;
		bCanPick = true;
	}
	virtual ~USpotlightComp() = default;
	EPrimitiveType GetType()override
	{
		return EPrimitiveType::EPT_Spotlight;
	}
};

class URingComp : public UPrimitiveComponent
{
	using Super = UPrimitiveComponent;
	DECLARE_CLASS(URingComp, UPrimitiveComponent)

public:
	URingComp()
	{
		bIsDefaultRendered = true;
		bCanPick = false;
	}
	virtual ~URingComp() = default;
	EPrimitiveType GetType()override
	{
		return EPrimitiveType::EPT_Ring;
	}
};
