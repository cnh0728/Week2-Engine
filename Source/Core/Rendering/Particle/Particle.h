#pragma once
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector.h"
#include "Core/Rendering/LegacyTexture.h"
#include "Core/AbstractClass/Singleton.h"
#include <d3d11.h>
#include <fstream>

class UParticle : public TSingleton<UParticle>
{
private:
	struct ParticleType
	{
		float PositionX, PositionY, PositionZ;
		bool Active;
		float LifeTime;
		float ScrollY, ScrollZ;
	};

	struct VertexType {
		FVector Position;
		FVector2 Texture;
		FVector4 Data;
	};
public:
	UParticle();
	~UParticle();

public:
	bool Create(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const wchar_t* ConfigFileName);
	void Release();
	bool Frame(float FrameTime, ID3D11DeviceContext* DeviceContext);
	void Render(ID3D11DeviceContext* DeviceContext);

	ID3D11ShaderResourceView* GetTexture();
	uint32 GetIndexCount();

	bool Reload(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);

private:
	bool LoadParticleConfiguration();

	bool CreateParticleSystem();
	void ReleaseParticleSystem();

	void EmitParticles(float FrameTime);
	void UpdateParticles(float FrameTime);
	void KillParticles();

	bool CreateBuffers(ID3D11Device* Device);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext* DeviceContext);
	bool UpdateBuffers(ID3D11DeviceContext* DeviceContext);

	bool LoadTexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);
	void ReleaseTexture();

private:
	ID3D11Buffer* VertexBuffer, * IndexBuffer;
	ParticleType* ParticleList;
	VertexType* Vertices;
	LegacyTexture* Texture;
	uint32 VertexCount, IndexCount;
	const wchar_t* ConfigFileName;
	uint32 MaxParticles;
	float ParticlesPerSecond;
	float ParticleSize;
	float ParticleLifeTime;
	wchar_t TextureFileName[256];
	float AccumulatedTime;
	float ElapsedTime = 5.0f;
	uint32 CurrentParticleCount;
};