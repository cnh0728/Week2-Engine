#pragma once

#include "Core/Engine.h"
#include "Object/USceneComponent.h"
#include "Primitive/PrimitiveVertices.h"
#include "Core/Math/Plane.h"


class UPrimitiveComponent : public USceneComponent
{
	using Super = USceneComponent;
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
	D3D11_PRIMITIVE_TOPOLOGY GetTopology() {
		switch (GetType())
		{
		case EPrimitiveType::EPT_Line:
		case EPrimitiveType::EPT_BoundingBox:
			return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		default:
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}
	}

	bool IsUseVertexColor() const { return bUseVertexColor; }

	void SetCustomColor(const FVector4& InColor)
	{
		CustomColor = InColor; 
		bUseVertexColor = false;
	}

	void SetUseVertexColor(bool bUse)
	{
		bUseVertexColor = bUse;
	}
	const FVector4& GetCustomColor() const { return CustomColor; }

public:
	virtual void RegisterComponentWithWorld(class UWorld* World);

public:
	void SetCanBeRendered(bool bRender) { bCanBeRendered = bRender; }
	bool GetCanBeRendered() const { return bCanBeRendered; }
	
	void SetIsOrthoGraphic(bool IsOrtho) { bIsOrthoGraphic = IsOrtho; }
	bool GetIsOrthoGraphic() { return bIsOrthoGraphic;}
	
protected:
	bool bCanBeRendered = false;
	bool bUseVertexColor = true;
	bool bIsOrthoGraphic = false;
	FVector4 CustomColor = FVector4(1.0f, 1.0f, 1.0f, 1.0f);

public:
	// UScenecomponent중에 PrimitiveComponent만 추출
	static TSet<UPrimitiveComponent*> FilterPrimitiveComponents(TSet<UActorComponent*>& Components);
};

class UCubeComp : public UPrimitiveComponent
{
	using Super = UPrimitiveComponent;
public:
	UCubeComp()
	{
		bCanBeRendered = true;
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
public:
	USphereComp()
	{
		bCanBeRendered = true;
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
public:
	UTriangleComp()
	{
		bCanBeRendered = true;
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

public:
	ULineComp()
	{
		bCanBeRendered = true;
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

public:
	UCylinderComp()
	{
		bCanBeRendered = true;
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
public:
	UConeComp()
	{
		bCanBeRendered = true;
	}
	virtual ~UConeComp() = default;
	EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Cone;
	}
};
