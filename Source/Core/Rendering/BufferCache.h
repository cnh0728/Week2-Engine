// #pragma once
//
// #define _TCHAR_DEFINED
// #include <d3d11.h>
//
// #include <wrl/client.h>
// #include <unordered_map>
// #include "Primitive/PrimitiveVertices.h"
// #include "Core/Container/Array.h"
// #include "Core/Container/Map.h"
//
// struct BufferInfo
// {
// public:
// 	BufferInfo() = default;
// 	BufferInfo(ID3D11Buffer* InBuffer, int BufferSize, D3D_PRIMITIVE_TOPOLOGY InTopology, const std::shared_ptr<FVertexSimple[]>& InVertices)
// 	{
// 		Buffer = InBuffer;
// 		Size = BufferSize;
// 		Topology = InTopology;
// 		Vertices = InVertices;
// 	}
// 	~BufferInfo()
// 	{
// 	}
//
// 	ID3D11Buffer* GetBuffer() const { return Buffer.Get(); }
// 	int GetSize() const { return Size; }
// 	D3D_PRIMITIVE_TOPOLOGY GetTopology() const { return Topology; }
// 	FVertexSimple* GetVertices() const;
//
// private:
// 	Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer;
// 	D3D_PRIMITIVE_TOPOLOGY Topology;
// 	int Size;
// 	std::shared_ptr<FVertexSimple[]> Vertices;
// };
//
// class FBufferCache
// {
// private:
// 	TMap<EPrimitiveType, BufferInfo> Cache;
//
// public:
// 	FBufferCache();
// 	~FBufferCache();
//
// 	void Init();
// 	BufferInfo GetBufferInfo(EPrimitiveType Type);
//
// public:
// 	// static TArray<FVertexSimple> CreateArrowVertices();
// 	static TArray<FVertexSimple> CreateConeVertices();
// 	static TArray<FVertexSimple> CreateCylinderVertices();
//
// private :
// 	BufferInfo CreateVertexBufferInfo(EPrimitiveType Type);
// };
//
