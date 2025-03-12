#include "BufferCache.h"
#include "Core/Engine.h"
#include "Primitive/PrimitiveVertices.h"

FBufferCache::FBufferCache()
{
	
}

FBufferCache::~FBufferCache()
{
}

void FBufferCache::Init()
{

}

BufferInfo FBufferCache::GetBufferInfo(EPrimitiveType Type)
{
	if (!Cache.Contains(Type))
	{
		//여기서 버텍스 버퍼 생성한다
		auto bufferInfo = CreateVertexBufferInfo(Type);
		Cache.Add(Type, bufferInfo);
	}

	return Cache[Type];
}

FVertexSimple* BufferInfo::GetVertices() const
{
	return Vertices.get();
}

BufferInfo FBufferCache::CreateVertexBufferInfo(EPrimitiveType Type)
{
	ID3D11Buffer* Buffer = nullptr;
	int Size = 0;
	D3D_PRIMITIVE_TOPOLOGY Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	std::shared_ptr<FVertexSimple[]> Vertices = nullptr;
	switch (Type)
	{
	case EPrimitiveType::EPT_Line:
		Vertices = std::make_shared<FVertexSimple[]>(sizeof(LineVertices));
		memcpy(Vertices.get(), LineVertices, sizeof(LineVertices));
		Size = std::size(LineVertices);
		Topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case EPrimitiveType::EPT_Triangle:
		Vertices = std::make_shared<FVertexSimple[]>(sizeof(TriangleVertices));
		memcpy(Vertices.get(), TriangleVertices, sizeof(TriangleVertices));
		Size = std::size(TriangleVertices);
		break;
	case EPrimitiveType::EPT_Cube:
		Vertices = std::make_shared<FVertexSimple[]>(sizeof(CubeVertices));
		memcpy(Vertices.get(), CubeVertices, sizeof(CubeVertices));
		Size = std::size(CubeVertices);
		break;
	case EPrimitiveType::EPT_Sphere:
		Vertices = std::make_shared<FVertexSimple[]>(sizeof(SphereVertices));
		memcpy(Vertices.get(), SphereVertices, sizeof(SphereVertices));
		Size = std::size(SphereVertices);
		break;
	case EPrimitiveType::EPT_Cylinder:
		{
			TArray<FVertexSimple> CylinderVertices = CreateCylinderVertices();
			Size = CylinderVertices.Num();
			Vertices = std::make_shared<FVertexSimple[]>(Size);
			memcpy(Vertices.get(), CylinderVertices.GetData(), sizeof(FVertexSimple) * Size);
			break;
		}
	case EPrimitiveType::EPT_Cone:
		{
			TArray<FVertexSimple> ConeVertices = CreateConeVertices();
			Size = ConeVertices.Num();
			Vertices = std::make_shared<FVertexSimple[]>(Size);
			memcpy(Vertices.get(), ConeVertices.GetData(), sizeof(FVertexSimple) * Size);
			break;
		}
	}

	Buffer = UEngine::Get().GetRenderer()->CreateVertexBuffer(Vertices.get(), sizeof(FVertexSimple) * Size);
	
	return BufferInfo(Buffer, Size, Topology, Vertices);
}


inline TArray<FVertexSimple> FBufferCache::CreateConeVertices()
{
	TArray<FVertexSimple> vertices;

	int segments = 36;
	float radius = 1.f;
	float height = 1.f;


	// 원뿔의 바닥
	for (int i = 0; i < segments; ++i)
	{
		float angle = 2.0f * PI * i / segments;
		float nextAngle = 2.0f * PI * (i + 1) / segments;

		float x1 = radius * cos(angle);
		float y1 = radius * sin(angle);
		float x2 = radius * cos(nextAngle);
		float y2 = radius * sin(nextAngle);

		 // 바닥 삼각형 (반시계 방향으로 추가)
        vertices.Add({ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f });
        vertices.Add({ x2, y2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f });
        vertices.Add({ x1, y1, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f });

        // 옆면 삼각형 (시계 방향으로 추가)
        vertices.Add({ x1, y1, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f });
        vertices.Add({ x2, y2, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f });
        vertices.Add({ 0.0f, 0.0f, height, 0.0f, 1.0f, 0.0f, 1.0f });
	}

	return vertices;
}

inline TArray<FVertexSimple> FBufferCache::CreateCylinderVertices()
{
	TArray<FVertexSimple> vertices;
	
	int segments = 36;
	float radius = .03f;
	float height = .5f;


	// 원기둥의 바닥과 윗면
	for (int i = 0; i < segments; ++i)
	{
		float angle = 2.0f * PI * i / segments;
		float nextAngle = 2.0f * PI * (i + 1) / segments;

		float x1 = radius * cos(angle);
		float y1 = radius * sin(angle);
		float x2 = radius * cos(nextAngle);
		float y2 = radius * sin(nextAngle);

		// 바닥 삼각형
		vertices.Add({ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f });
		vertices.Add({ x2, y2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f });
		vertices.Add({ x1, y1, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f });

		// 윗면 삼각형
		vertices.Add({ 0.0f, 0.0f, height, 0.0f, 1.0f, 0.0f, 1.0f });
		vertices.Add({ x1, y1, height, 0.0f, 1.0f, 0.0f, 1.0f });
		vertices.Add({ x2, y2, height, 0.0f, 1.0f, 0.0f, 1.0f });

		// 옆면 삼각형 두 개
		vertices.Add({ x1, y1, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f });
		vertices.Add({ x2, y2, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f });
		vertices.Add({ x1, y1, height, 0.0f, 0.0f, 1.0f, 1.0f });

		vertices.Add({ x2, y2, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f });
		vertices.Add({ x2, y2, height, 0.0f, 0.0f, 1.0f, 1.0f });
		vertices.Add({ x1, y1, height, 0.0f, 0.0f, 1.0f, 1.0f });
	}

	return vertices;
}
