#include "Particle.h"
#include <Debug/DebugConsole.h>

UParticle::UParticle()
{
	VertexBuffer = nullptr;
	IndexBuffer = nullptr;
	ParticleList = nullptr;
	Vertices = nullptr;
	Texture = nullptr;
	VertexCount = 0;
	IndexCount = 0;
}

UParticle::~UParticle()
{
	Release();
}

bool UParticle::Create(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const wchar_t* ConfigFileName)
{
	this->ConfigFileName = ConfigFileName;
	bool Result = LoadParticleConfiguration();
    if (!Result)
    {
		UE_LOG("LoadParticleConfiguration Fault!");
        return false;
    }

	Result = CreateParticleSystem();
	if (!Result)
	{
		UE_LOG("CreateParticleSystem Fault!");
		return false;
	}

	Result = CreateBuffers(Device);
	if (!Result)
	{
		UE_LOG("CreateBuffers Fault!");
		return false;
	}

	Result = LoadTexture(Device, DeviceContext);
	if (!Result)
	{
		UE_LOG("LoadTexture Fault!");
		return false;
	}
	return true;
}

void UParticle::Release()
{
	ReleaseTexture();
	ReleaseBuffers();
	ReleaseParticleSystem();
	ElapsedTime = 0.0f;

	return;
}

bool UParticle::Frame(float FrameTime, ID3D11DeviceContext* DeviceContext)
{
	ElapsedTime += FrameTime;
	if (ElapsedTime < ParticleLifeTime)
	{
		EmitParticles(FrameTime);
		KillParticles();
	}

	UpdateParticles(FrameTime);
	UpdateBuffers(DeviceContext);

    return false;
}

void UParticle::Render(ID3D11DeviceContext* DeviceContext)
{
	RenderBuffers(DeviceContext);
		
	return;
}

ID3D11ShaderResourceView* UParticle::GetTexture()
{
	return Texture->GetTexture();
}

uint32 UParticle::GetIndexCount()
{
	return IndexCount;
}

bool UParticle::Reload(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{
	bool Result;

	Release();

	Result = LoadParticleConfiguration();
	if (!Result)
	{
		return false;
	}

	Result = CreateParticleSystem();
	if (!Result)
	{
		return false;
	}

	Result = CreateBuffers(Device);
	if (!Result)
	{
		return false;
	}

	Result = LoadTexture(Device, DeviceContext);
	if (!Result)
	{
		return false;
	}
    
	return true;
}

bool UParticle::LoadParticleConfiguration()
{
    std::ifstream fin;
    int i;
    char input;


	// Config 파일 열기
    fin.open(ConfigFileName);
    if (fin.fail())
    {
        return false;
    }

	// MaxParticles, ParticlesPerSecond, ParticleSize, ParticleLifeTime, TextureFileName 읽기
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    fin >> MaxParticles;

    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    fin >> ParticlesPerSecond;

    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    fin >> ParticleSize;

    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    fin >> ParticleLifeTime;

    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    fin.get(input);

    i = 0;
    fin.get(input);
    while (input != '\n')
    {
        TextureFileName[i] = input;
        i++;
        fin.get(input);
    }
    TextureFileName[i] = '\0';

    // 파일 닫기
    fin.close();

    return true;
}

bool UParticle::CreateParticleSystem()
{
	ParticleList = new ParticleType[MaxParticles];

	// 파티클 생성 시간 초기화
	AccumulatedTime = 0.0f;

	// 현재 파티클 개수 초기화
	CurrentParticleCount = 0;

    return true;
}

void UParticle::ReleaseParticleSystem()
{
    if (ParticleList)
    {
		delete[] ParticleList;
		ParticleList = nullptr;
    }

    return;
}

void UParticle::EmitParticles(float FrameTime)
{
	if (CurrentParticleCount > MaxParticles - 1)
		return;

    float CenterY, CenterZ;
	float PositionX, PositionY, PositionZ;
    float Radius;
    float ScrollY, ScrollZ;
    float EmitterOrigin[3];
	bool EmitParticle, Found;
    static float Angle = 0.0f;

    CenterY = 0.0f;
    CenterZ = 0.0f;

    Radius = 0.7f;

	// 매 프레임마다 생성된 파티클의 원점 위치를 원의 둘레를 따라 이동시킴
    Angle += FrameTime * 3.0f;

    // 파티클이 원의 둘레에서 방출되어야 할 원점을 계산
	EmitterOrigin[0] = 0.0f;
	EmitterOrigin[1] = CenterY + Radius * sin(Angle);
	EmitterOrigin[2] = CenterZ + Radius * cos(Angle);

	// 새 파티클을 방출할 위치를 계산
	PositionX = EmitterOrigin[0];
	PositionY = EmitterOrigin[1];
	PositionZ = EmitterOrigin[2];

	// Y 방향의 랜덤한 양수 스크롤 값을 생성
	ScrollY = (((float)rand() - (float)rand()) / RAND_MAX);
	if (ScrollY < 0.0f)
	{
		ScrollY *= -1.0f;
	}

	// Y도 X와 동일하게
	ScrollZ = ScrollY;

	int index = CurrentParticleCount++;
	auto& p = ParticleList[index];
	p.LifeTime = ParticleLifeTime;
	p.PositionX = EmitterOrigin[0];
	p.PositionY = EmitterOrigin[1];
	p.PositionZ = EmitterOrigin[2];
	p.ScrollY = ScrollY;
	p.ScrollZ = ScrollZ;

	return;
   
}

void UParticle::UpdateParticles(float FrameTime)
{
	for (int i = 0; i < CurrentParticleCount; i++)
	{
		ParticleList[i].LifeTime = ParticleList[i].LifeTime - FrameTime;

		ParticleList[i].ScrollY = ParticleList[i].ScrollY + (FrameTime * 0.5f);
		if (ParticleList[i].ScrollY > 1.0f)
		{
			ParticleList[i].ScrollY -= 1.0f;
		}

		ParticleList[i].ScrollZ = ParticleList[i].ScrollZ + (FrameTime * 0.5f);
		if (ParticleList[i].ScrollZ > 1.0f)
		{
			ParticleList[i].ScrollZ -= 1.0f;
		}
	}

	return;
}

void UParticle::KillParticles()
{
	uint32 TempCurrentParticleCount = CurrentParticleCount;
	for (int i = 0; i < TempCurrentParticleCount; i++)
	{
		if (ParticleList[i].LifeTime <= 0.0f)
		{
			CurrentParticleCount--;
		}
	}

	return;
}

bool UParticle::CreateBuffers(ID3D11Device* Device)
{
	ULONG* Indices;
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData, IndexData;
	HRESULT Result;

	// 렌더링할 파티클의 최대 정점(Vertex) 개수를 설정
	VertexCount = MaxParticles * 6;

	// 정점과 인덱스의 1대1 매칭
	// 렌더링 파이프라인의 일관성 유지
	IndexCount = VertexCount;

	// 렌더링할 파티클을 담은 버텍스 배열 생성
	Vertices = new VertexType[VertexCount];

	// 인덱스 배열 생성
	Indices = new ULONG[IndexCount];

	// 초기화
	memset(Vertices, 0, (sizeof(VertexType) * VertexCount));
	for (int i = 0; i < IndexCount; i++)
	{
		Indices[i] = i;
	}

	// 버텍스 버퍼 옵션 세팅
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	Result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &VertexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// 인덱스 버퍼 옵션 세팅
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(ULONG) * IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &IndexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	delete[] Indices;
	Indices = nullptr;

    return true;
}

void UParticle::ReleaseBuffers()
{
	if (IndexBuffer)
	{
		IndexBuffer->Release();
		IndexBuffer = nullptr;
	}
	if (VertexBuffer)
	{
		VertexBuffer->Release();
		VertexBuffer = nullptr;
	}
	if (Vertices)
	{
		delete[] Vertices;
		Vertices = nullptr;
	}
	return;
}

void UParticle::RenderBuffers(ID3D11DeviceContext* DeviceContext)
{
	uint32 Stride;
	uint32 Offset;

	Stride = sizeof(VertexType);
	Offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
	
	DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	return;
}

bool UParticle::UpdateBuffers(ID3D11DeviceContext* DeviceContext)
{
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	VertexType* VerticesPtr;
	float LifeTime, ScrollY, ScorllZ;
	
	memset(Vertices, 0, (sizeof(VertexType) * VertexCount));

	int index = 0;

	for (int i = 0; i < CurrentParticleCount; i++)
	{
		LifeTime = ParticleList[i].LifeTime / ParticleLifeTime;
		ScrollY = ParticleList[i].ScrollY;
		ScorllZ = ParticleList[i].ScrollZ;

		// 왼쪽 아래
		Vertices[index].Position = FVector(ParticleList[i].PositionX, ParticleList[i].PositionY - ParticleSize, ParticleList[i].PositionZ - ParticleSize);
		Vertices[index].Texture = FVector2(0.0f, 1.0f);
		Vertices[index].Data = FVector4(LifeTime, ScrollY, ScorllZ, 1.0f);
		index++;

		// 왼쪽 위
		Vertices[index].Position = FVector(ParticleList[i].PositionX, ParticleList[i].PositionY - ParticleSize, ParticleList[i].PositionZ + ParticleSize);
		Vertices[index].Texture = FVector2(0.0f, 0.0f);
		Vertices[index].Data = FVector4(LifeTime, ScrollY, ScorllZ, 1.0f);
		index++;

		// 오른쪽 아래
		Vertices[index].Position = FVector(ParticleList[i].PositionX, ParticleList[i].PositionY + ParticleSize, ParticleList[i].PositionZ - ParticleSize);
		Vertices[index].Texture = FVector2(1.0f, 1.0f);
		Vertices[index].Data = FVector4(LifeTime, ScrollY, ScorllZ, 1.0f);
		index++;

		// 오른쪽 아래
		Vertices[index].Position = FVector(ParticleList[i].PositionX, ParticleList[i].PositionY + ParticleSize, ParticleList[i].PositionZ - ParticleSize);
		Vertices[index].Texture = FVector2(1.0f, 1.0f);
		Vertices[index].Data = FVector4(LifeTime, ScrollY, ScorllZ, 1.0f);
		index++;

		// 왼쪽 위
		Vertices[index].Position = FVector(ParticleList[i].PositionX, ParticleList[i].PositionY - ParticleSize, ParticleList[i].PositionZ + ParticleSize);
		Vertices[index].Texture = FVector2(0.0f, 0.0f);
		Vertices[index].Data = FVector4(LifeTime, ScrollY, ScorllZ, 1.0f);
		index++;

		// 오른쪽 위
		Vertices[index].Position = FVector(ParticleList[i].PositionX, ParticleList[i].PositionY + ParticleSize, ParticleList[i].PositionZ + ParticleSize);
		Vertices[index].Texture = FVector2(1.0f, 0.0f);
		Vertices[index].Data = FVector4(LifeTime, ScrollY, ScorllZ, 1.0f);
		index++;
	}

	Result = DeviceContext->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		return false;
	}

	VerticesPtr = (VertexType*)MappedResource.pData;

	memcpy(VerticesPtr, (void*)Vertices, (sizeof(VertexType) * VertexCount));
	DeviceContext->Unmap(VertexBuffer, 0);

	return true;
}

bool UParticle::LoadTexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{
	bool Result;

	Texture = new LegacyTexture();
	Result = Texture->Create(Device, TextureFileName);
	if (FAILED(Result))
	{
		return false;
	}

    return true;
}

void UParticle::ReleaseTexture()
{
	if (Texture)
	{
		Texture->Release();
		delete Texture;
		Texture = nullptr;
	}
	return;
}
