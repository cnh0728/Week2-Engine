#include "PrimitiveVertices.h"

#include "Core/Engine.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

TMap<EPrimitiveType, TArray<FVertexPNCT>> OriginVertices = {
	{
		EPrimitiveType::EPT_Line,{
				{ -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f , -1.0, 0.0f, 0.0f, 0.0f, 0.0f},
				{  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f , 1.0, 0.0f, 0.0f, 1.0f, 1.0f}
		}
	}
};
TMap<EPrimitiveType, TArray<uint32_t>> OriginIndices = {
	{
		EPrimitiveType::EPT_Line, {0, 1}
	},
};

void FVertexPNCT::CreateOriginVertices()
{
	CreateCubeVertices();
	CreateCylinderVertices();
	CreateTriangleVertices();
	CreateRingVertices();
	CreateConeVertices();
	CreateSphereVertices();
	CreateTextureBoardVertices();
	CreateBoundingBoxVertices();
	CreateSpotlightVertices();
}

void FVertexPNCT::CreateCubeVertices()
{
	TArray<FVertexPNCT> Vertices;
	TArray<uint32_t> Indices;
	
	// Z- 앞면
	Vertices.Add({ -0.5f, -0.5f, -0.5f,0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f , 0.0f, 0.0f });  // Bottom-left
	Vertices.Add({ -0.5f, 0.5f,  -0.5f,0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f , 1.0f, 0.0f });   // Top-left (yellow)
	Vertices.Add({ 0.5f,  0.5f,  -0.5f,0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f });  // Top-right (blue))
	Vertices.Add({ 0.5f, -0.5f,  -0.5f,0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f });  // Bottom-right (green)
		// Back face (Z+)
	Vertices.Add({ 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,-1.0f,  0.0f, 1.0f, 1.0f, 1.0f,0.0f, 0.0f});  // Bottom-left (cyan)
	Vertices.Add({ 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,-1.0f,  0.0f, 0.0f, 1.0f, 1.0f,1.0f, 0.0f});  // Top-left (blue)
	Vertices.Add({ -0.5f,  0.5f, 0.5f, 0.0f, 0.0f,-1.0f,  1.0f, 1.0f, 0.0f, 1.0f ,1.0f, 1.0f});  // Top-right (yellow)
	Vertices.Add({ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,-1.0f,  1.0f, 0.0f, 1.0f, 1.0f ,0.0f, 1.0f});  // Bottom-right (magenta)

		// Left face (X-)
	Vertices.Add({ -0.5f, -0.5f,  0.5f, -1.0f,0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f,0.0f, 0.0f}); // Bottom-left (purple)
	Vertices.Add({ -0.5f,  0.5f,  0.5f, -1.0f,0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,1.0f, 0.0f}); // Top-left (blue)
	Vertices.Add({ -0.5f,  0.5f,  -0.5f, -1.0f,0.0f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f,1.0f, 1.0f}); // Top-right (yellow)
	Vertices.Add({ -0.5f, -0.5f,  -0.5f, -1.0f,0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,0.0f, 1.0f}); // Bottom-right (green)

		// Right face (X+)
	Vertices.Add({ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  1.0f, 0.5f, 0.0f, 1.0f,0.0f, 0.0f});  // Bottom-left (orange)
	Vertices.Add({ 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.5f, 0.0f, 0.5f, 1.0f,1.0f, 0.0f});  // Top-left (purple)
	Vertices.Add({ 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.5f, 1.0f,1.0f, 1.0f});  // Top-right (dark blue)
	Vertices.Add({ 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  0.5f, 0.5f, 0.5f, 1.0f,0.0f, 1.0f});  // Bottom-right (gray)
																	  
		// Top face (Y+)											  
	Vertices.Add({ -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.5f, 1.0f,0.0f, 0.0f });  // Bottom-left (light green)
	Vertices.Add({ -0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 0.5f, 1.0f, 1.0f,1.0f, 0.0f});  // Top-left (cyan)
	Vertices.Add({ 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  0.5f, 0.5f, 0.0f, 1.0f ,1.0f, 1.0f});  // Top-right (brown)
	Vertices.Add({ 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.5f, 1.0f, 1.0f, 1.0f ,0.0f, 1.0f});  // Bottom-right (white)
																	  
		// Bottom face (Y-)											  
	Vertices.Add({ 0.5f,  -0.5f, -0.5f, 0.0f, -1.0f,0.0f,  0.5f, 0.5f, 0.0f, 1.0f,0.0f, 0.0f});  // Bottom-left (brown)
	Vertices.Add({ 0.5f,  -0.5f,  0.5f, 0.0f, -1.0f,0.0f,  1.0f, 0.0f, 0.0f, 1.0f,1.0f, 0.0f});  // Top-left (red)
	Vertices.Add({ -0.5f,  -0.5f,  0.5f, 0.0f, -1.0f,0.0f,  0.0f, 1.0f, 0.0f, 1.0f ,1.0f, 1.0f});  // Top-right (green)
	Vertices.Add({ -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,0.0f,  1.0f, 0.0f, 0.5f, 1.0f ,0.0f, 1.0f});  // Bottom-right (purple)

	Indices = {
		0,  1,  2,  0,  2,  3,  // 윗면
		4,  5,  6,  4,  6,  7,  // 아랫면
		8,  9,  10, 8,  10, 11, // 앞면
		12, 13, 14, 12, 14, 15, // 뒷면
		16, 17, 18, 16, 18, 19, // 왼쪽
		20, 21, 22, 20, 22, 23  // 오른쪽
	};

	EPrimitiveType Type = EPrimitiveType::EPT_Cube;
	
	OriginVertices[Type] = Vertices;
	OriginIndices[Type] = Indices;

	URenderer* Renderer = UEngine::Get().GetRenderer();
	VertexBufferInfo BufferInfo = {UPrimitiveComponent::GetTopology(Type), Vertices, Indices};
	// //Renderer->CreateVertexBuffer(Type, BufferInfo);
}

void FVertexPNCT::CreateCylinderVertices()
{
	TArray<FVertexPNCT> Vertices;
	TArray<uint32_t> Indices;
	TArray<uint32_t> TempIndices;
	int segments = 36;
	float radius = 0.03f;
	float height = 0.5f;

	// Bottom and top center points
	uint32 bottomCenterIndex = Vertices.Num();
	Vertices.Add({ 0.0f, 0.0f, 0.0f,0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f });
	uint32 topCenterIndex = Vertices.Num();
	Vertices.Add({ 0.0f, 0.0f, height,0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f });

	for (int i = 0; i < segments; ++i)
	{
		float angle = 2.0f * PI * i / segments;
		float nextAngle = 2.0f * PI * (i + 1) / segments;

		float x1 = radius * cos(angle);
		float y1 = radius * sin(angle);
		float x2 = radius * cos(nextAngle);
		float y2 = radius * sin(nextAngle);
		float u1 = static_cast<float>(i) / segments;
		float u2 = static_cast<float>(i + 1) / segments;

		float nx1 = cos(angle);
		float ny1 = sin(angle);
		float nx2 = cos(nextAngle);
		float ny2 = sin(nextAngle);

		// Bottom face
		uint32 v1 = Vertices.Num();
		Vertices.Add({ x1, y1, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, x1 * 0.5f + 0.5f, y1 * 0.5f + 0.5f });
		uint32 v2 = Vertices.Num();
		Vertices.Add({ x2, y2, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, x2 * 0.5f + 0.5f, y2 * 0.5f + 0.5f });
		TempIndices = { bottomCenterIndex, v2, v1 };
		Indices.Insert(Indices.end(), TempIndices.begin(), TempIndices.end());

		// Top face
		uint32 v3 = Vertices.Num();
		Vertices.Add({ x1, y1, height, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, x1 * 0.5f + 0.5f, y1 * 0.5f + 0.5f });
		uint32 v4 = Vertices.Num();
		Vertices.Add({ x2, y2, height, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, x2 * 0.5f + 0.5f, y2 * 0.5f + 0.5f });
		TempIndices = { topCenterIndex, v3, v4 };
		Indices.Insert(Indices.end(), TempIndices.begin(), TempIndices.end());

		// Side faces with duplicated vertices for correct normals and UV mapping
		uint32 v5 = Vertices.Num();
		Vertices.Add({ x1, y1, 0.0f, nx1, ny1, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, u1, 0.0f });
		uint32 v6 = Vertices.Num();
		Vertices.Add({ x1, y1, height, nx1, ny1, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, u1, 1.0f });
		uint32 v7 = Vertices.Num();
		Vertices.Add({ x2, y2, 0.0f, nx2, ny2, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, u2, 0.0f });
		uint32 v8 = Vertices.Num();
		Vertices.Add({ x2, y2, height, nx2, ny2, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, u2, 1.0f });

		TempIndices = { v5, v7, v6 };
		Indices.Insert(Indices.end(), TempIndices.begin(), TempIndices.end());
		TempIndices = { v7, v8, v6 };
		Indices.Insert(Indices.end(), TempIndices.begin(), TempIndices.end());
	}

	EPrimitiveType Type = EPrimitiveType::EPT_Cylinder;
	
	OriginVertices[Type] = Vertices;
	OriginIndices[Type] = Indices;

	URenderer* Renderer = UEngine::Get().GetRenderer();
	VertexBufferInfo BufferInfo = {UPrimitiveComponent::GetTopology(Type), Vertices, Indices};
	//Renderer->CreateVertexBuffer(Type, BufferInfo);

}

void FVertexPNCT::CreateTriangleVertices()
{
	TArray<FVertexPNCT> Vertices;
	TArray<uint32_t> Indices;
	TArray<uint32_t> TempIndices;

	float size = 0.5f; // 한 변의 길이
	float height = sqrt(3.0f) / 2.0f * size; // 정삼각형 높이

	// vertex
	FVector v0 = { 0.0f,  height / 2.0f, 0.0f };
	FVector v1 = { -size / 2.0f, -height / 2.0f, 0.0f };
	FVector v2 = { size / 2.0f, -height / 2.0f, 0.0f };

	// front_normal - z+
	FVector normalFront = { 0.0f, 0.0f, 1.0f };

	// backfront_normal - z-
	FVector normalBack = { 0.0f, 0.0f, -1.0f };

	// front_face
	uint32 i0 = Vertices.Num();
	Vertices.Add({ v0.X, v0.Y, v0.Z, normalFront.X, normalFront.Y, normalFront.Z, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f });
	uint32 i1 = Vertices.Num();
	Vertices.Add({ v1.X, v1.Y, v1.Z, normalFront.X, normalFront.Y, normalFront.Z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f });
	uint32 i2 = Vertices.Num();
	Vertices.Add({ v2.X, v2.Y, v2.Z, normalFront.X, normalFront.Y, normalFront.Z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f });

	//
	TempIndices = { i0, i1, i2 };
	Indices.Insert(Indices.end(), TempIndices.begin(), TempIndices.end());

	//back_face
	uint32 i3 = Vertices.Num();
	Vertices.Add({ v0.X, v0.Y, v0.Z, normalBack.X, normalBack.Y, normalBack.Z, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f });
	uint32 i4 = Vertices.Num();
	Vertices.Add({ v2.X, v2.Y, v2.Z, normalBack.X, normalBack.Y, normalBack.Z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f });
	uint32 i5 = Vertices.Num();
	Vertices.Add({ v1.X, v1.Y, v1.Z, normalBack.X, normalBack.Y, normalBack.Z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f });

	TempIndices = { i3, i4, i5 };
	Indices.Insert(Indices.end(), TempIndices.begin(), TempIndices.end());

	EPrimitiveType Type = EPrimitiveType::EPT_Triangle;
	
	OriginVertices[Type] = Vertices;
	OriginIndices[Type] = Indices;

	URenderer* Renderer = UEngine::Get().GetRenderer();
	VertexBufferInfo BufferInfo = {UPrimitiveComponent::GetTopology(Type), Vertices, Indices};
	//Renderer->CreateVertexBuffer(Type, BufferInfo);

}

void FVertexPNCT::CreateRingVertices()
{
	TArray<FVertexPNCT> Vertices;
	TArray<uint32_t> Indices;
	TArray<uint32_t> TempIndices;

	int DISC_RESOLUTION = 128; // 원을 구성하는 정점 개수
	float outerRadius = 1.0f;  // 외곽 반지름
	float innerRadius = 0.9f;  // 내부 반지름
	float height = 0.1f;       
	float angleStep = 2.0f * PI / DISC_RESOLUTION;

	uint32 topCenterIndex = Vertices.Num();
	Vertices.Add({ 0.0f, height / 2, 0.0f, 0, 1, 0, 1, 1, 1, 1,0.5f, 0.5f });
	uint32 bottomCenterIndex = Vertices.Num();
	Vertices.Add({ 0.0f, -height / 2, 0.0f, 0, -1, 0, 1, 1, 1, 1,0.5f, 0.5f });

	for (int i = 0; i < DISC_RESOLUTION; ++i)
	{
		float angle = i * angleStep;
		float nextAngle = (i + 1) * angleStep;

		float x0 = cos(angle);
		float z0 = sin(angle);
		float x1 = cos(nextAngle);
		float z1 = sin(nextAngle);

		float u0 = x0 * 0.5f + 0.5f;
		float v0 = z0 * 0.5f + 0.5f;
		float u1 = x1 * 0.5f + 0.5f;
		float v1 = z1 * 0.5f + 0.5f;

		// 위쪽 원면 (탑)
		uint32 v1_t = Vertices.Num();
		Vertices.Add({ x0 * outerRadius, height / 2, z0 * outerRadius, 0, 1, 0, 1, 1, 1, 1, u0, v0 });
		uint32 v2_t = Vertices.Num();
		Vertices.Add({ x0 * innerRadius, height / 2, z0 * innerRadius, 0, 1, 0, 1, 1, 1, 1, u0, v0 });
		uint32 v3_t = Vertices.Num();
		Vertices.Add({ x1 * outerRadius, height / 2, z1 * outerRadius, 0, 1, 0, 1, 1, 1, 1, u1, v1 });
		uint32 v4_t = Vertices.Num();
		Vertices.Add({ x1 * innerRadius, height / 2, z1 * innerRadius, 0, 1, 0, 1, 1, 1, 1, u1, v1 });

		TempIndices = { v1_t, v2_t, v3_t, v2_t, v4_t, v3_t };
		Indices.Insert(Indices.end(), TempIndices.begin(), TempIndices.end());

		// 바닥면 (Bottom)
		uint32 v1_b = Vertices.Num();
		Vertices.Add({ x0 * outerRadius, -height / 2, z0 * outerRadius, 0, -1, 0, 1, 1, 1, 1, u0, v0 });
		uint32 v2_b = Vertices.Num();
		Vertices.Add({ x0 * innerRadius, -height / 2, z0 * innerRadius, 0, -1, 0, 1, 1, 1, 1, u0, v0 });
		uint32 v3_b = Vertices.Num();
		Vertices.Add({ x1 * outerRadius, -height / 2, z1 * outerRadius, 0, -1, 0, 1, 1, 1, 1, u1, v1 });
		uint32 v4_b = Vertices.Num();
		Vertices.Add({ x1 * innerRadius, -height / 2, z1 * innerRadius, 0, -1, 0, 1, 1, 1, 1, u1, v1 });

		TempIndices = { v3_b, v2_b, v1_b, v3_b, v4_b, v2_b };
		Indices.Insert(Indices.end(), TempIndices.begin(), TempIndices.end());

		// 외곽 측면
		uint32 v1_s = Vertices.Num();
		Vertices.Add({ x0 * outerRadius, height / 2, z0 * outerRadius, x0, 0, z0, 1, 1, 1, 1, u0, 1.0f });
		uint32 v2_s = Vertices.Num();
		Vertices.Add({ x1 * outerRadius, height / 2, z1 * outerRadius, x1, 0, z1, 1, 1, 1, 1, u1, 1.0f });
		uint32 v3_s = Vertices.Num();
		Vertices.Add({ x0 * outerRadius, -height / 2, z0 * outerRadius, x0, 0, z0, 1, 1, 1, 1, u0, 0.0f });
		uint32 v4_s = Vertices.Num();
		Vertices.Add({ x1 * outerRadius, -height / 2, z1 * outerRadius, x1, 0, z1, 1, 1, 1, 1, u1, 0.0f });

		TempIndices = { v1_s,v2_s,v3_s, v2_s,v4_s, v3_s, };
		Indices.Insert(Indices.end(), TempIndices.begin(), TempIndices.end());

		// 내부 측면
		uint32 v1_i = Vertices.Num();
		Vertices.Add({ x0 * innerRadius, height / 2, z0 * innerRadius, -x0, 0, -z0,1, 1, 1, 1,  u0, 1.0f });
		uint32 v2_i = Vertices.Num();
		Vertices.Add({ x1 * innerRadius, height / 2, z1 * innerRadius, -x1, 0, -z1,1, 1, 1, 1,  u1, 1.0f });
		uint32 v3_i = Vertices.Num();
		Vertices.Add({ x0 * innerRadius, -height / 2, z0 * innerRadius, -x0, 0, -z0,1, 1, 1, 1,  u0, 0.0f });
		uint32 v4_i = Vertices.Num();
		Vertices.Add({ x1 * innerRadius, -height / 2, z1 * innerRadius, -x1, 0, -z1,1, 1, 1, 1,  u1, 0.0f });

		TempIndices = { v1_i, v3_i,v2_i, v2_i,  v3_i,v4_i };
		Indices.Insert(Indices.end(), TempIndices.begin(), TempIndices.end());
	}

	EPrimitiveType Type = EPrimitiveType::EPT_Ring;
	OriginVertices[Type]=Vertices;
	OriginIndices[Type]=Indices;

	URenderer* Renderer = UEngine::Get().GetRenderer();
	VertexBufferInfo BufferInfo = {UPrimitiveComponent::GetTopology(Type), Vertices, Indices};
	//Renderer->CreateVertexBuffer(Type, BufferInfo);

}

void FVertexPNCT::CreateConeVertices()
{
	TArray<FVertexPNCT> Vertices;
	TArray<uint32_t> Indices;

	int segments = 36;
	float radius = 0.5f;
	float height = 1.0f;

	// 원뿔의 바닥 원의 중심 점
	Vertices.Add({ 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f,  
		0.5f, 0.5f });

	for (int i = 0; i <= segments; ++i)
	{
		float angle = 2.0f * PI * i / segments;

		float x = radius * cos(angle);
		float y = radius * sin(angle);

		// 바닥 Vertex 추가
		Vertices.Add({ x, y, 0.0f,  
			0.0f, - 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.5f + cos(angle) / 2.0f, 0.5f - sin(angle) / 2.0f });
	}

	// 바닥 원 (반시계 방향으로 추가)
	for (int i = 1; i <= segments; i++)
	{
		Indices.Add(0);
		Indices.Add(i + 1);
		Indices.Add(i);
	}

	int baseIndex = segments + 2;

	// 옆면 삼각형 Vertex 추가
	for (int i = 0; i < segments; ++i)
	{
		float angle = 2.0f * PI * i / segments;
		float nextAngle = 2.0f * PI * (i + 1) / segments;

		float x1 = radius * cos(angle);
		float y1 = radius * sin(angle);
		float x2 = radius * cos(nextAngle);
		float y2 = radius * sin(nextAngle);

		// 바닥 Vertex 2개, 꼭대기 Vertex 1개 위치
		FVector v1 = { x1, y1, 0.0f };
		FVector v2 = { x2, y2, 0.0f };
		FVector h = { 0.0f, 0.0f, height };

		// 3점의 위치를 통해 평면의 normal 벡터 구하기
		FVector n = FVector::ComputeNormalFromThreePoint(v1, v2, h);

		// 바닥 Vertex 추가
		Vertices.Add({ v1.X, v1.Y, v1.Z,  
			n.X, n.Y, n.Z,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.5f + (cos(-angle / 2.0f) / 2.0f), 0.5f + (sin(-angle / 2.0f) / 2.0f) });
		// 꼭지 Vertex 추가
		Vertices.Add({ h.X, h.Y, h.Z, 
			n.X, n.Y, n.Z,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.0f });

		// 옆면의 경우 UV가 반원의 형태이기에 시작점과 끝점이 같은 위치로 해야함
		if (i == segments - 1)
		{
			Vertices.Add({ v2.X, v2.Y, v2.Z,  
			n.X, n.Y, n.Z,
				1.0f, 1.0f, 1.0f, 1.0f,
			0.5f + (cos(-nextAngle / 2.0f) / 2.0f), 0.5f + (sin(-nextAngle / 2.0f) / 2.0f) });
		}
	}

	// 옆면 삼각형 엮기
	for (int i = 0; i < 2 * segments - 1; )
	{
		Indices.Add(baseIndex + i);
		Indices.Add(baseIndex + i + 2);
		Indices.Add(baseIndex + i + 1);
		
		i += 2;
	}

	EPrimitiveType Type = EPrimitiveType::EPT_Cone;
	OriginVertices[Type]=Vertices;
	OriginIndices[Type]=Indices;

	URenderer* Renderer = UEngine::Get().GetRenderer();
	VertexBufferInfo BufferInfo = {UPrimitiveComponent::GetTopology(Type), Vertices, Indices};
	//Renderer->CreateVertexBuffer(Type, BufferInfo);

}

void FVertexPNCT::CreateSphereVertices()
{
	TArray<FVertexPNCT> Vertices;
	TArray<uint32_t> Indices;
	TArray<uint32_t> TempIndices;

	int segments = 36;
	float radius = 0.5f;

	// 구의 꼭대기 파트
	for (int i = 0; i < segments; ++i) {
		float theta = 2.0f * PI * i / segments;
		float nextTheta = 2.0f * PI * (i + 1) / segments;
		// 맨 위 꼭짓점 Vertex 추가 Texture을 위해 여러 개
		Vertices.Add({ 0.0f, 0.0f, radius,
			0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			(theta + nextTheta) / (4.0f * PI), 0.0f }); // theta 평균 내서 사용
	}

	// 중간 부분을 이루는 점들
	for (int i = 1; i < segments; ++i)
	{
		float phi = PI * i / segments;
		float z = radius * cos(phi);
		float rSin = radius * sin(phi);
		for (int j = 0; j <= segments; ++j)
		{
			float theta = 2.0f * PI * j / segments;
			float x = rSin * cos(theta);
			float y = rSin * sin(theta);

			Vertices.Add({ x, y, z, 
				x, y, z, // 구의 특성을 살려 Normal Vector로 활용
				theta / (2.0f * PI), 1.0f, 1.0f, 1.0f,
				theta / (2.0f * PI), phi / PI });
		}
	}

	// 구의 아래 파트
	for (int i = 0; i < segments; ++i) {
		float theta = 2.0f * PI * i / segments;
		float nextTheta = 2.0f * PI * (i + 1) / segments;
		// 맨 아래 꼭짓점 Vertex 추가
		Vertices.Add({ 0.0f, 0.0f, -radius, 
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			(theta + nextTheta) / (4.0f * PI), 1.0f }); // theta 평균 내서 사용
	}

	// 구의 윗 꼭짓점과 삼각형 엮기
	for (int i = 0; i < segments; ++i) {
		Indices.Add(i);
		Indices.Add(i + segments);
		Indices.Add(i + segments + 1);
	}

	// 구의 중간 부분 삼각형 2번 엮어서 사각형 만들기
	int baseIndex = segments;
	for (int i = 0; i < segments - 2; ++i) {    // 가로로 한바퀴씩 돌리는 것을 몇번할지
		for (int j = 0; j < segments; ++j) {
			Indices.Add(baseIndex + j);
			Indices.Add(baseIndex + j + (segments + 1));
			Indices.Add(baseIndex + j + (segments + 2));

			Indices.Add(baseIndex + j);
			Indices.Add(baseIndex + j + (segments + 2));
			Indices.Add(baseIndex + j + 1);

		}
		baseIndex += segments + 1;
	}

	// 구의 아래 꼭짓점 과 삼각형 엮기
	for (int i = 0; i < segments; ++i) {
		Indices.Add(baseIndex + i);
		Indices.Add(baseIndex + segments + 1 + i);
		Indices.Add(baseIndex + i + 1);
	}

	EPrimitiveType Type = EPrimitiveType::EPT_Sphere;

	OriginVertices[Type]=Vertices;
	OriginIndices[Type]=Indices;

	URenderer* Renderer = UEngine::Get().GetRenderer();
	VertexBufferInfo BufferInfo = {UPrimitiveComponent::GetTopology(Type), Vertices, Indices};
	//Renderer->CreateVertexBuffer(Type, BufferInfo);
}

void FVertexPNCT::CreateTextureBoardVertices()
{
	TArray<FVertexPNCT> Vertices;
	TArray<uint32> Indices;

	Vertices.Add({ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,1.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f });  // Bottom-left (orange)
	Vertices.Add({ 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f });  // Top-left (purple)
	Vertices.Add({ 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f });  // Top-right (dark blue)
	Vertices.Add({ 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f });  // Bottom-right (gray)

	Indices = {
		0,  1,  2,  0,  2,  3,
	};

	EPrimitiveType Type = EPrimitiveType::EPT_Texture;
	
	OriginVertices[Type]=Vertices;
	OriginIndices[Type]=Indices;

	URenderer* Renderer = UEngine::Get().GetRenderer();
	VertexBufferInfo BufferInfo = {UPrimitiveComponent::GetTopology(Type), Vertices, Indices};
	//Renderer->CreateVertexBuffer(Type, BufferInfo);
}

void FVertexPNCT::CreateBoundingBoxVertices()
{
	TArray<FVertexPNCT> Vertices;
	TArray<uint32> Indices;
	// 모두 하얀색의 선을 그리는 정육면체
	// Z-Up 왼손 좌표계(언리얼 엔진) 기준 큐브 정점
	Vertices.Add({ -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }); // 0: 전면(-Y) 좌하단
	Vertices.Add({ -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }); // 1: 전면(-Y) 좌상단
	Vertices.Add({ 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }); // 2: 전면(-Y) 우상단
	Vertices.Add({ 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }); // 3: 전면(-Y) 우하단
	Vertices.Add({ -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }); // 4: 후면(+Y) 좌하단
	Vertices.Add({ -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,1.0f, 1.0f, 1.0f, 1.0f }); // 5: 후면(+Y) 좌상단
	Vertices.Add({ 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }); // 6: 후면(+Y) 우상단
	Vertices.Add({ 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }); // 7: 후면(+Y) 우하단

	Indices = {
		// 전면 (앞면, -Y 방향)
		0, 1,  1, 2,  2, 3,  3, 0,
		// 후면 (뒷면, +Y 방향)
		4, 5,  5, 6,  6, 7,  7, 4,
		// 좌우 연결선
		0, 4,  1, 5,  2, 6,  3, 7
	};

	EPrimitiveType Type = EPrimitiveType::EPT_BoundingBox;
	OriginVertices[Type]=Vertices;
	OriginIndices[Type]=Indices;
	
	URenderer* Renderer = UEngine::Get().GetRenderer();
	VertexBufferInfo BufferInfo = {UPrimitiveComponent::GetTopology(Type), Vertices, Indices};
	Renderer->CreateVertexBuffer(Type, BufferInfo);
}

void FVertexPNCT::CreateSpotlightVertices()
{
	TArray<FVertexPNCT> Vertices;
	TArray<uint32_t> Indices;

	int segments = 128;
	float radius = 0.5f;
	float height = 1.0f;

	// spotlight의 원점(광원)
	Vertices.Add({ 0,0, 0, 0.0f, 0.0f, 0.0f,1.0f,1.0f,1.0f,1.0f });

	for (int i = 0; i <= segments; ++i)
	{
		float angle = 2.0f * PI * i / segments;

		float x = radius * cos(angle);
		float y = radius * sin(angle);
		float z = height;

		// 바닥 Vertex 추가
		Vertices.Add({ x,y,z,0.0f, 0.0f, 0.0f, 1.0f,1.0f,1.0f,1.0f });
	}

	for (int i = 1; i < segments + 1; i++)
	{
		Indices.Add(0);
		Indices.Add(i);
		Indices.Add(i);
		Indices.Add(i+1);
		Indices.Add(i+1);
		Indices.Add(0);
	}

	// normal은 없습니다.

	EPrimitiveType Type = EPrimitiveType::EPT_Spotlight;
	OriginVertices[Type]=Vertices;
	OriginIndices[Type]=Indices;
	
	URenderer* Renderer = UEngine::Get().GetRenderer();
	VertexBufferInfo BufferInfo = {UPrimitiveComponent::GetTopology(Type), Vertices, Indices};
	Renderer->CreateVertexBuffer(Type, BufferInfo);
}
