#include "URenderer.h"
#include <d3dcompiler.h>
#include "Core/Math/Transform.h"
#include <Object/Actor/Camera.h>

#include "Object/Actor/WorldGrid.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Static/FEditorManager.h"
#include "../FSlateApplication.h"

void VertexBufferInfo::AddVertices(TArray<FVertexSimple> InVertices, TArray<uint32_t> InIndices)
{
	Vertices.Insert(Vertices.end(), InVertices.begin(), InVertices.end());
	VertexCount = Vertices.Num();

	Indices.Insert(Indices.end(), InIndices.begin(), InIndices.end());
	IndexCount = Indices.Num();
}

void VertexBufferInfo::ClearVertices()
{
	Vertices = {};
	VertexCount = 0;

	Indices = {};
	IndexCount = 0;
}

URenderer::~URenderer()
{
	Release();
}

void URenderer::Create(HWND hWindow)
{
	CreateDeviceAndSwapChain(hWindow);
	CreateMultipleViewports();		//MultiViewport
	CreateFrameBuffer();
	CreateRasterizerState();
	// CreateBufferCache();
	CreateDepthStencilBuffer();
	CreateDepthStencilState();
	CreatePickingTexture(hWindow);

	FVertexSimple::CreateOriginVertices();

	CreateText(hWindow);
	CreateAlphaBlendingState();

	InitMatrix();

	CreateParticle(hWindow);
	CreateTexture(hWindow);
}

void URenderer::Release()
{
	ReleaseRasterizerState();

	// 렌더 타겟을 초기화
	DeviceContext->OMSetRenderTargets(0, nullptr, DepthStencilView);

	ReleaseFrameBuffer();
	ReleaseDepthStencilBuffer();
	ReleaseDeviceAndSwapChain();
	ReleaseAllVertexBuffer();
	ReleaseAlphaBlendingState();
	ReleaseMultiViewport();
}

void URenderer::CreateShader()
{
	/**
		 * 컴파일된 셰이더의 바이트코드를 저장할 변수 (ID3DBlob)
		 *
		 * 범용 메모리 버퍼를 나타내는 형식
		 *   - 여기서는 shader object bytecode를 담기위해 쓰임
		 * 다음 두 메서드를 제공한다.
		 *   - LPVOID GetBufferPointer
		 *     - 버퍼를 가리키는 void* 포인터를 돌려준다.
		 *   - SIZE_T GetBufferSize
		 *     - 버퍼의 크기(바이트 갯수)를 돌려준다
		 */
	ID3DBlob* VertexShaderCSO;
	ID3DBlob* PixelShaderCSO;

	ID3DBlob* PickingShaderCSO;

	ID3DBlob* ErrorMsg = nullptr;
	// 셰이더 컴파일 및 생성
	D3DCompileFromFile(L"Shaders/ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &VertexShaderCSO, &ErrorMsg);

	Device->CreateVertexShader(VertexShaderCSO->GetBufferPointer(), VertexShaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

	D3DCompileFromFile(L"Shaders/ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &PixelShaderCSO, &ErrorMsg);
	Device->CreatePixelShader(PixelShaderCSO->GetBufferPointer(), PixelShaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

	D3DCompileFromFile(L"Shaders/ShaderW0.hlsl", nullptr, nullptr, "PickingPS", "ps_5_0", 0, 0, &PickingShaderCSO, nullptr);
	Device->CreatePixelShader(PickingShaderCSO->GetBufferPointer(), PickingShaderCSO->GetBufferSize(), nullptr, &PickingPixelShader);

	if (ErrorMsg)
	{
		std::cout << (char*)ErrorMsg->GetBufferPointer() << std::endl;
		ErrorMsg->Release();
	}

	// 입력 레이아웃 정의 및 생성
	D3D11_INPUT_ELEMENT_DESC Layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		// 인스턴스 데이터 (Per-Instance)
		// { "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		// { "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		// { "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		// { "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	Device->CreateInputLayout(Layout, ARRAYSIZE(Layout), VertexShaderCSO->GetBufferPointer(), VertexShaderCSO->GetBufferSize(), &SimpleInputLayout);

	VertexShaderCSO->Release();
	PixelShaderCSO->Release();
	PickingShaderCSO->Release();

	// 정점 하나의 크기를 설정 (바이트 단위)
	Stride = sizeof(FVertexSimple);
}

void URenderer::ReleaseShader()
{
	if (SimpleInputLayout)
	{
		SimpleInputLayout->Release();
		SimpleInputLayout = nullptr;
	}

	if (SimplePixelShader)
	{
		SimplePixelShader->Release();
		SimplePixelShader = nullptr;
	}

	if (SimpleVertexShader)
	{
		SimpleVertexShader->Release();
		SimpleVertexShader = nullptr;
	}
}

void URenderer::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC ConstantBufferDesc = {};
	ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;                        // 매 프레임 CPU에서 업데이트 하기 위해
	ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;             // 상수 버퍼로 설정
	ConstantBufferDesc.ByteWidth = sizeof(FConstants) + 0xf & 0xfffffff0;  // 16byte의 배수로 올림
	ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;            // CPU에서 쓰기 접근이 가능하게 설정

	Device->CreateBuffer(&ConstantBufferDesc, nullptr, &ConstantBuffer);

	D3D11_BUFFER_DESC ConstantBufferDescPicking = {};
	ConstantBufferDescPicking.Usage = D3D11_USAGE_DYNAMIC;                        // 매 프레임 CPU에서 업데이트 하기 위해
	ConstantBufferDescPicking.BindFlags = D3D11_BIND_CONSTANT_BUFFER;             // 상수 버퍼로 설정
	ConstantBufferDescPicking.ByteWidth = sizeof(FPickingConstants) + 0xf & 0xfffffff0;  // 16byte의 배수로 올림
	ConstantBufferDescPicking.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;            // CPU에서 쓰기 접근이 가능하게 설정

	Device->CreateBuffer(&ConstantBufferDescPicking, nullptr, &ConstantPickingBuffer);

	D3D11_BUFFER_DESC ConstantBufferDescDepth = {};
	ConstantBufferDescPicking.Usage = D3D11_USAGE_DYNAMIC;                        // 매 프레임 CPU에서 업데이트 하기 위해
	ConstantBufferDescPicking.BindFlags = D3D11_BIND_CONSTANT_BUFFER;             // 상수 버퍼로 설정
	ConstantBufferDescPicking.ByteWidth = sizeof(FDepthConstants) + 0xf & 0xfffffff0;  // 16byte의 배수로 올림
	ConstantBufferDescPicking.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;            // CPU에서 쓰기 접근이 가능하게 설정

	Device->CreateBuffer(&ConstantBufferDescPicking, nullptr, &ConstantsDepthBuffer);
}

void URenderer::ReleaseConstantBuffer()
{
	if (ConstantBuffer)
	{
		ConstantBuffer->Release();
		ConstantBuffer = nullptr;
	}

	if (ConstantPickingBuffer)
	{
		ConstantPickingBuffer->Release();
		ConstantPickingBuffer = nullptr;
	}

	if (ConstantsDepthBuffer)
	{
		ConstantsDepthBuffer->Release();
		ConstantsDepthBuffer = nullptr;
	}
}

void URenderer::SwapBuffer() const
{
	SwapChain->Present(1, 0); // SyncInterval: VSync 활성화 여부
}

void URenderer::Prepare()
{
	// 화면 지우기
	DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor);
	DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// DeviceContext->

	// InputAssembler의 Vertex 해석 방식을 설정
	// DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //왜인진 모르겠는데 매프레임 해줘야하나봄.

	// Rasterization할 Viewport를 설정 
	DeviceContext->RSSetViewports(1, &ViewportInfo);

	DeviceContext->RSSetState(RasterizerState);

	/**
	 * OutputMerger 설정
	 * 렌더링 파이프라인의 최종 단계로써, 어디에 그릴지(렌더 타겟)와 어떻게 그릴지(블렌딩)를 지정
	 */
	DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, DepthStencilView);    // DepthStencil 뷰 설정
	DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

void URenderer::PrepareShader() const
{
	// 기본 셰이더랑 InputLayout을 설정
	DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
	DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
	DeviceContext->IASetInputLayout(SimpleInputLayout);

	// 버텍스 쉐이더에 상수 버퍼를 설정
	if (ConstantBuffer)
	{
		DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
	}
}

TMap<D3D11_PRIMITIVE_TOPOLOGY, bool> URenderer::CheckChangedVertexCount()
{
	TMap<D3D11_PRIMITIVE_TOPOLOGY, bool> Topologies;

	for (auto& Pair : BatchVertexBuffers)
	{
		auto& Key = Pair.Key;

		VertexBufferInfo BufferInfo = BatchVertexBuffers[Key];

		if (BufferInfo.GetVertexCount() > 0)
		{
			if (BufferInfo.GetVertexCount() != BufferInfo.GetVertexPreCount())
			{
				Topologies.Add(Key, true);
			}
			else
			{
				Topologies.Add(Key, false);
			}
		}
	}

	return Topologies;
}

void URenderer::RenderBatch()
{
	//Pending으로 돌면서 버텍스 갯수 달라졌으면 resize, 같으면 동적할당으로 버텍스버퍼 업데이트

	//TODO: 배치 액터별로 하고 바꾸기 지금은 그리드만 있으니까 임시용

	UpdateConstant(FEditorManager::Get().GetWorldGrid()->GetRootComponent());

	for (auto& Pair : BatchVertexBuffers)
	{
		auto& Key = Pair.Key;
		auto& Value = Pair.Value;

		if (CurrentTopology != Value.GetTopology())
		{
			D3D11_PRIMITIVE_TOPOLOGY Topology = Value.GetTopology();
			DeviceContext->IASetPrimitiveTopology(Topology);
			CurrentTopology = Topology;
		}

		//그려야하느게 없으면 컨티뉴
		if (Value.GetVertexCount() == 0)
		{
			continue;
		}

		ID3D11Buffer* VertexBuffer = Value.GetVertexBuffer();
		ID3D11Buffer* IndexBuffer = Value.GetIndexBuffer();

		UINT Offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
		DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		DeviceContext->DrawIndexed(Value.GetIndexCount(), 0, 0);
	}
}

void URenderer::CreateVertexBuffer(EPrimitiveType VertexType, VertexBufferInfo BufferInfo)
{
	if (VertexBuffers.Contains(VertexType))
	{
		return;
	}

	TArray<FVertexSimple> Vertices = BufferInfo.GetVertices();
	FVertexSimple* RawVertices = Vertices.GetData();
	// D3D11_PRIMITIVE_TOPOLOGY Topology = BufferInfo.GetTopology();
	D3D11_PRIMITIVE_TOPOLOGY Topology = UPrimitiveComponent::GetTopology(VertexType);
	uint32_t VertexByteWidth = BufferInfo.GetVertexCount() * sizeof(FVertexSimple);

	if (VertexByteWidth == 0)
	{
		return;
	}

	D3D11_BUFFER_DESC VertexBufferDesc = {};
	VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	VertexBufferDesc.ByteWidth = VertexByteWidth;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA VertexBufferSRD = {};
	VertexBufferSRD.pSysMem = RawVertices;

	//UUID받으면 설정이 빈 버퍼 만들어주기
	ID3D11Buffer* VertexBuffer;
	Device->CreateBuffer(&VertexBufferDesc, &VertexBufferSRD, &VertexBuffer);


	TArray<uint32_t> Indices = BufferInfo.GetIndices();
	uint32_t* RawIndices = Indices.GetData();

	uint32_t IndexByteWidth = BufferInfo.GetIndexCount() * sizeof(uint32_t);

	if (IndexByteWidth == 0)
	{
		return;
	}

	D3D11_BUFFER_DESC IndexBufferDesc = {};
	IndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	IndexBufferDesc.ByteWidth = IndexByteWidth;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = RawIndices;

	ID3D11Buffer* IndexBuffer;
	Device->CreateBuffer(&IndexBufferDesc, &indexData, &IndexBuffer);

	VertexBufferInfo NewBufferInfo = { VertexBuffer, IndexBuffer, Topology, Vertices, Indices };
	SetVertexBufferInfo(VertexType, NewBufferInfo);
}

void URenderer::CreateBatchVertexBuffer(D3D11_PRIMITIVE_TOPOLOGY Topology, VertexBufferInfo BufferInfo)
{
	TArray<FVertexSimple> Vertices = BufferInfo.GetVertices();
	FVertexSimple* RawVertices = Vertices.GetData();
	// D3D11_PRIMITIVE_TOPOLOGY Topology = BufferInfo.GetTopology();

	uint32_t VertexByteWidth = BufferInfo.GetVertexCount() * sizeof(FVertexSimple);

	if (VertexByteWidth == 0)
	{
		return;
	}

	D3D11_BUFFER_DESC VertexBufferDesc = {};
	VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	VertexBufferDesc.ByteWidth = VertexByteWidth;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA VertexBufferSRD = {};
	VertexBufferSRD.pSysMem = RawVertices;

	//UUID받으면 설정이 빈 버퍼 만들어주기
	ID3D11Buffer* VertexBuffer;
	Device->CreateBuffer(&VertexBufferDesc, &VertexBufferSRD, &VertexBuffer);


	TArray<uint32_t> Indices = BufferInfo.GetIndices();
	uint32_t* RawIndices = Indices.GetData();

	uint32_t IndexByteWidth = BufferInfo.GetIndexCount() * sizeof(uint32_t);

	if (IndexByteWidth == 0)
	{
		return;
	}

	D3D11_BUFFER_DESC IndexBufferDesc = {};
	IndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	IndexBufferDesc.ByteWidth = IndexByteWidth;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = RawIndices;

	ID3D11Buffer* IndexBuffer;
	Device->CreateBuffer(&IndexBufferDesc, &indexData, &IndexBuffer);

	VertexBufferInfo NewBufferInfo = { VertexBuffer, IndexBuffer, Topology, Vertices, Indices };
	SetBatchVertexBufferInfo(Topology, NewBufferInfo);
}

void URenderer::ClearBatchVertex()
{
	for (auto& Pair : BatchVertexBuffers)
	{
		auto& Value = Pair.Value;

		if (Value.GetVertexCount() > 0)
		{
			Value.ClearVertices();
		}
	}
}

void URenderer::AddBatchVertices(UPrimitiveComponent* Component)
{
	//자기 컴포넌트 매트릭스 곱해서 버텍스 ADD하기
	//월드매트릭스만 해주고 V, P는 Constant로 넘기기 (Primitivie별로 곱해줄 필요없으니까)
	// FMatrix WorldMatrix = Component->GetComponentTransformMatrix();

	//월드매트릭스만 해주고 V, P는 Constant로 넘기기 (Primitivie별로 곱해줄 필요없으니까

	D3D11_PRIMITIVE_TOPOLOGY Topology = Component->GetTopology(Component->GetType());

	// uint32_t UUID = Component->GetOwner()->GetUUID();
	TArray<FVertexSimple> Vertices = OriginVertices[Component->GetType()];
	TArray<uint32_t> Indices = OriginIndices[Component->GetType()];

	FMatrix ComponentWorldMatrix = Component->GetComponentTransformMatrix();

	for (FVertexSimple& Vertex : Vertices)
	{
		Vertex.SetPos(ComponentWorldMatrix);
		if (Component->bCustomColor)
		{
			FVector4 Color = Component->GetColor();
			Vertex.SetColor(Color);
		}
	}

	VertexBufferInfo& BufferInfo = BatchVertexBuffers[Topology];
	uint32_t IndexOffset = BufferInfo.GetVertexCount();
	for (uint32_t& Index : Indices)
	{
		Index += IndexOffset;
	}

	BufferInfo.AddVertices(Vertices, Indices);
}

VertexBufferInfo URenderer::ResizeBatchVertexBuffer(D3D11_PRIMITIVE_TOPOLOGY Topology)
{
	if (BatchVertexBuffers.Contains(Topology) == false)
	{
		return VertexBufferInfo();
	}

	VertexBufferInfo BufferInfo = BatchVertexBuffers[Topology]; //버텍스버퍼를 제외한 이전 정보 저장용

	ReleaseVertexBuffer(Topology);

	CreateBatchVertexBuffer(Topology, BufferInfo);

	return BufferInfo;
}

void URenderer::UpdateBatchVertexBuffer()
{
	TMap<D3D11_PRIMITIVE_TOPOLOGY, bool> ChangedType = CheckChangedVertexCount();

	for (auto& pair : ChangedType)
	{
		auto& Topology = pair.Key;
		auto& IsVertexCountChanged = pair.Value;

		if (IsVertexCountChanged)
		{
			ResizeBatchVertexBuffer(Topology);
		}
	}
}

void URenderer::ReleaseVertexBuffer(D3D11_PRIMITIVE_TOPOLOGY Topology)
{
	if (BatchVertexBuffers[Topology].GetVertexBuffer() != nullptr)
	{
		BatchVertexBuffers[Topology].GetVertexBuffer()->Release();
		BatchVertexBuffers[Topology].GetIndexBuffer()->Release();
		BatchVertexBuffers.Remove(Topology);
	}
}

void URenderer::RenderPrimtivie(UPrimitiveComponent* Component)
{
	if (VertexBuffers.Contains(Component->GetType()) == false)
	{
		return;
	}

	VertexBufferInfo Info = VertexBuffers[Component->GetType()];

	if (Info.GetVertexBuffer() == nullptr)
	{
		return;
	}

	if (CurrentTopology != Info.GetTopology())
	{
		DeviceContext->IASetPrimitiveTopology(Info.GetTopology());
		CurrentTopology = Info.GetTopology();
	}

	UpdateConstant(Component);

	ID3D11Buffer* VertexBuffer = Info.GetVertexBuffer();
	ID3D11Buffer* IndexBuffer = Info.GetIndexBuffer();
	uint32_t IndexCount = Info.GetIndexCount();

	UINT Offset = 0;
	DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
	DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	DeviceContext->DrawIndexed(IndexCount, 0, 0);
}

void URenderer::ReleaseAllVertexBuffer()
{
	for (auto& pair : BatchVertexBuffers)
	{
		auto& Key = pair.Key;
		auto& Value = pair.Value;

		if (Value.GetVertexBuffer() != nullptr)
		{
			BatchVertexBuffers[Key].GetVertexBuffer()->Release();
			BatchVertexBuffers[Key].GetIndexBuffer()->Release();
		}
	}
	BatchVertexBuffers.Empty();
}

void URenderer::UpdateConstant(USceneComponent* Component) const
{
	if (!ConstantBuffer) return;

	ACamera* Camera = FEditorManager::Get().GetCamera();

	D3D11_MAPPED_SUBRESOURCE ConstantBufferMSR;

	FMatrix M = FMatrix::Transpose(Component->GetComponentTransformMatrix());
	FMatrix V = FMatrix::Transpose(ViewMatrix);
	FMatrix P = FMatrix::Transpose(ProjectionMatrix);

	// D3D11_MAP_WRITE_DISCARD는 이전 내용을 무시하고 새로운 데이터로 덮어쓰기 위해 사용
	DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ConstantBufferMSR);
	{
		// 매핑된 메모리를 FConstants 구조체로 캐스팅
		FConstants* Constants = static_cast<FConstants*>(ConstantBufferMSR.pData);
		Constants->M = M;
		Constants->V = V;
		Constants->P = P;
		Constants->Color = Component->GetColor();
		Constants->bUseCustomColor = Component->bCustomColor;
	}
	DeviceContext->Unmap(ConstantBuffer, 0);
}


ID3D11Device* URenderer::GetDevice() const
{
	return Device;
}

ID3D11DeviceContext* URenderer::GetDeviceContext() const
{
	return DeviceContext;
}

void URenderer::CreateDeviceAndSwapChain(HWND hWindow)
{
	// 지원하는 Direct3D 기능 레벨을 정의
	D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	// SwapChain 구조체 초기화
	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
	SwapChainDesc.BufferDesc.Width = 0;                            // 창 크기에 맞게 자동으로 설정
	SwapChainDesc.BufferDesc.Height = 0;                           // 창 크기에 맞게 자동으로 설정
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 색상 포멧
	SwapChainDesc.SampleDesc.Count = 1;                            // 멀티 샘플링 비활성화
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // 렌더 타겟으로 설정
	SwapChainDesc.BufferCount = 2;                                 // 더블 버퍼링
	SwapChainDesc.OutputWindow = hWindow;                          // 렌더링할 창 핸들
	SwapChainDesc.Windowed = TRUE;                                 // 창 모드
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;      // 스왑 방식

	// Direct3D Device와 SwapChain을 생성
	D3D11CreateDeviceAndSwapChain(
		// 입력 매개변수
		nullptr,                                                       // 디바이스를 만들 때 사용할 비디오 어댑터에 대한 포인터
		D3D_DRIVER_TYPE_HARDWARE,                                      // 만들 드라이버 유형을 나타내는 D3D_DRIVER_TYPE 열거형 값
		nullptr,                                                       // 소프트웨어 래스터라이저를 구현하는 DLL에 대한 핸들
		D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,  // 사용할 런타임 계층을 지정하는 D3D11_CREATE_DEVICE_FLAG 열거형 값들의 조합
		FeatureLevels,                                                 // 만들려는 기능 수준의 순서를 결정하는 D3D_FEATURE_LEVEL 배열에 대한 포인터
		ARRAYSIZE(FeatureLevels),                                      // pFeatureLevels 배열의 요소 수
		D3D11_SDK_VERSION,                                             // SDK 버전. 주로 D3D11_SDK_VERSION을 사용
		&SwapChainDesc,                                                // SwapChain 설정과 관련된 DXGI_SWAP_CHAIN_DESC 구조체에 대한 포인터

		// 출력 매개변수
		&SwapChain,                                                    // 생성된 IDXGISwapChain 인터페이스에 대한 포인터
		&Device,                                                       // 생성된 ID3D11Device 인터페이스에 대한 포인터
		nullptr,                                                       // 선택된 기능 수준을 나타내는 D3D_FEATURE_LEVEL 값을 반환
		&DeviceContext                                                 // 생성된 ID3D11DeviceContext 인터페이스에 대한 포인터
	);

	// 생성된 SwapChain의 정보 가져오기
	SwapChain->GetDesc(&SwapChainDesc);

	// 뷰포트 정보 설정
	ViewportInfo = {
		0.0f, 0.0f,
		static_cast<float>(SwapChainDesc.BufferDesc.Width), static_cast<float>(SwapChainDesc.BufferDesc.Height),
		0.0f, 1.0f
	};

#ifdef _DEBUG

	HMODULE hPixGpuCapturer = LoadLibraryA("WinPixGpuCapturer.dll");
	if (!hPixGpuCapturer)
	{
		OutputDebugStringA("Failed to load WinPixGpuCapturer.dll\n");
	}
#endif
}

void URenderer::ReleaseDeviceAndSwapChain()
{
	if (DeviceContext)
	{
		DeviceContext->Flush(); // 남이있는 GPU 명령 실행
	}

	if (SwapChain)
	{
		SwapChain->Release();
		SwapChain = nullptr;
	}

	if (Device)
	{
		Device->Release();
		Device = nullptr;
	}

	if (DeviceContext)
	{
		DeviceContext->Release();
		DeviceContext = nullptr;
	}
}

void URenderer::CreateFrameBuffer()
{
	// 스왑 체인으로부터 백 버퍼 텍스처 가져오기
	SwapChain->GetBuffer(0, IID_PPV_ARGS(&FrameBuffer));

	// 렌더 타겟 뷰 생성
	D3D11_RENDER_TARGET_VIEW_DESC FrameBufferRTVDesc = {};
	FrameBufferRTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;      // 색상 포맷
	FrameBufferRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D 텍스처

	Device->CreateRenderTargetView(FrameBuffer, &FrameBufferRTVDesc, &FrameBufferRTV);
}

void URenderer::CreateDepthStencilBuffer()
{
	D3D11_TEXTURE2D_DESC DepthBufferDesc = {};
	DepthBufferDesc.Width = static_cast<UINT>(ViewportInfo.Width);
	DepthBufferDesc.Height = static_cast<UINT>(ViewportInfo.Height);
	DepthBufferDesc.MipLevels = 1; 								     // 미리 계산된 텍스쳐 레벨의 수
	DepthBufferDesc.ArraySize = 1; 								     // 텍스쳐 배열의 크기
	DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;          // 32비트 중 24비트는 깊이, 8비트는 스텐실
	DepthBufferDesc.SampleDesc.Count = 1; 						     // 멀티 샘플링을 사용하지 않음
	DepthBufferDesc.SampleDesc.Quality = 0; 				         // 멀티 샘플링을 사용하지 않음
	DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT; 					 // GPU에서 읽기/쓰기 가능
	DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;            // 텍스쳐 바인딩 플래그를 DepthStencil로 설정
	DepthBufferDesc.CPUAccessFlags = 0; 							 // CPU에서 접근하지 않음
	DepthBufferDesc.MiscFlags = 0; 								     // 기타 플래그

	HRESULT result = Device->CreateTexture2D(&DepthBufferDesc, nullptr, &DepthStencilBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DepthBufferDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	result = Device->CreateDepthStencilView(DepthStencilBuffer, &dsvDesc, &DepthStencilView);
}

void URenderer::CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc = {};
	DepthStencilDesc.DepthEnable = TRUE;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;                     // 더 작은 깊이값이 왔을 때 픽셀을 갱신함
	// DepthStencilDesc.StencilEnable = FALSE;                                 // 스텐실 테스트는 하지 않는다.
	// DepthStencilDesc.StencilReadMask = 0xFF;
	// DepthStencilDesc.StencilWriteMask = 0xFF;
	// DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	// DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	// DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	// DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	// DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	// DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	// DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	Device->CreateDepthStencilState(&DepthStencilDesc, &DepthStencilState);

	D3D11_DEPTH_STENCIL_DESC IgnoreDepthStencilDesc = {};
	DepthStencilDesc.DepthEnable = FALSE;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	Device->CreateDepthStencilState(&IgnoreDepthStencilDesc, &IgnoreDepthStencilState);
}

void URenderer::ReleaseFrameBuffer()
{
	if (FrameBuffer)
	{
		FrameBuffer->Release();
		FrameBuffer = nullptr;
	}

	if (FrameBufferRTV)
	{
		FrameBufferRTV->Release();
		FrameBufferRTV = nullptr;
	}

	if (PickingFrameBuffer)
	{
		PickingFrameBuffer->Release();
		PickingFrameBuffer = nullptr;
	}

	if (PickingFrameBufferRTV)
	{
		PickingFrameBufferRTV->Release();
		PickingFrameBufferRTV = nullptr;
	}
}

void URenderer::ReleaseDepthStencilBuffer()
{
	if (DepthStencilBuffer)
	{
		DepthStencilBuffer->Release();
		DepthStencilBuffer = nullptr;
	}
	if (DepthStencilView)
	{
		DepthStencilView->Release();
		DepthStencilView = nullptr;
	}
	if (DepthStencilState)
	{
		DepthStencilState->Release();
		DepthStencilState = nullptr;
	}
	if (IgnoreDepthStencilState)
	{
		IgnoreDepthStencilState->Release();
		IgnoreDepthStencilState = nullptr;
	}
}

void URenderer::CreateRasterizerState()
{
	if (RasterizerState)
	{
		RasterizerState->Release();
		RasterizerState = nullptr;
	}

	D3D11_RASTERIZER_DESC RasterizerDesc = {};
	RasterizerDesc.FillMode = CurrentFillMode;
	RasterizerDesc.CullMode = D3D11_CULL_BACK;
	RasterizerDesc.FrontCounterClockwise = FALSE;

	HRESULT hr = Device->CreateRasterizerState(&RasterizerDesc, &RasterizerState);
	if (FAILED(hr))
		UE_LOG("failed for create rasterizer state");
	else
		DeviceContext->RSSetState(RasterizerState);

}

void URenderer::ReleaseRasterizerState()
{
	if (RasterizerState)
	{
		RasterizerState->Release();
		RasterizerState = nullptr;
	}
}

// void URenderer::CreateBufferCache()
// {
//     BufferCache = std::make_unique<FBufferCache>();
// }

void URenderer::InitMatrix()
{
	WorldMatrix = FMatrix::Identity();
	ViewMatrix = FMatrix::Identity();
	ProjectionMatrix = FMatrix::Identity();
}

void URenderer::CreateMultipleViewports()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	SwapChain->GetDesc(&swapChainDesc);

	float fullWidth = static_cast<float>(swapChainDesc.BufferDesc.Width);
	float fullHeight = static_cast<float>(swapChainDesc.BufferDesc.Height);
	float halfWidth = fullWidth / 2.0f;
	float halfHeight = fullHeight / 2.0f;

	D3D11_VIEWPORT viewports[4] = {};

	// 왼쪽 상단
	viewports[0] = { 0.0f, 0.0f, halfWidth, halfHeight, 0.0f, 1.0f };
	// 오른쪽 상단
	viewports[1] = { halfWidth, 0.0f, halfWidth, halfHeight, 0.0f, 1.0f };
	// 왼쪽 하단
	viewports[2] = { 0.0f, halfHeight, halfWidth, halfHeight, 0.0f, 1.0f };
	// 오른쪽 하단
	viewports[3] = { halfWidth, halfHeight, halfWidth, halfHeight, 0.0f, 1.0f };


	FRect rectLT;
	rectLT.Min = FVector2(0, 0);
	rectLT.Max = FVector2(halfWidth, halfHeight);
	FViewport* viewportLT = new FViewport();
	viewportLT->SetSWindow(FSlateApplication::Get().SNEW(rectLT));
	viewportLT->UpdateSWindowSize();
	MultiFViewports.Add(viewportLT);

	FRect rectRT;
	rectRT.Min = FVector2(halfWidth, 0);
	rectRT.Max = FVector2(fullWidth, halfHeight);
	FViewport* viewportRT = new FViewport();
	viewportRT->SetSWindow(FSlateApplication::Get().SNEW(rectRT));
	viewportRT->UpdateSWindowSize();
	MultiFViewports.Add(viewportRT);

	FRect rectLB;
	rectLB.Min = FVector2(0, halfHeight);
	rectLB.Max = FVector2(halfWidth, fullHeight);
	FViewport* viewportLB = new FViewport();
	viewportLB->SetSWindow(FSlateApplication::Get().SNEW(rectLB));
	viewportLB->UpdateSWindowSize();
	MultiFViewports.Add(viewportLB);

	FRect rectRB;
	rectRB.Min = FVector2(halfWidth, halfHeight);
	rectRB.Max = FVector2(fullWidth, fullHeight);
	FViewport* viewportRB = new FViewport();
	viewportRB->SetSWindow(FSlateApplication::Get().SNEW(rectRB));
	viewportRB->UpdateSWindowSize();
	MultiFViewports.Add(viewportRB);


	MulitViewports.Add(viewports[0]);
	MulitViewports.Add(viewports[1]);
	MulitViewports.Add(viewports[2]);
	MulitViewports.Add(viewports[3]);

	// 뷰포트 설정 적용
	//DeviceContext->RSSetViewports(4, viewports);
}

const TArray<FViewport*>& URenderer::GetActiveViewport()
{
	return MultiFViewports;
}

void URenderer::SetMultiViewport(int index)
{
	DeviceContext->RSSetViewports(1, &MulitViewports[index]);
}

void URenderer::ReleaseMultiViewport()
{
	for (int i = 0; i < MultiFViewports.Num(); i++)
	{
		delete MultiFViewports[i];
	}
}




void URenderer::ReleasePickingFrameBuffer()
{
	if (PickingFrameBuffer)
	{
		PickingFrameBuffer->Release();
		PickingFrameBuffer = nullptr;
	}
	if (PickingFrameBufferRTV)
	{
		PickingFrameBufferRTV->Release();
		PickingFrameBufferRTV = nullptr;
	}
}

void URenderer::CreatePickingTexture(HWND hWnd)
{
	RECT Rect;
	int Width, Height;

	Width = ViewportInfo.Width;
	Height = ViewportInfo.Height;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = Width;
	textureDesc.Height = Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	Device->CreateTexture2D(&textureDesc, nullptr, &PickingFrameBuffer);

	D3D11_RENDER_TARGET_VIEW_DESC PickingFrameBufferRTVDesc = {};
	PickingFrameBufferRTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      // 색상 포맷
	PickingFrameBufferRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D 텍스처

	Device->CreateRenderTargetView(PickingFrameBuffer, &PickingFrameBufferRTVDesc, &PickingFrameBufferRTV);
}

void URenderer::PrepareZIgnore()
{
	DeviceContext->OMSetDepthStencilState(IgnoreDepthStencilState, 0);
}

void URenderer::PreparePicking()
{
	// 렌더 타겟 바인딩
	DeviceContext->OMSetRenderTargets(1, &PickingFrameBufferRTV, nullptr);
	DeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	DeviceContext->OMSetDepthStencilState(DepthStencilState, 0);                // DepthStencil 상태 설정. StencilRef: 스텐실 테스트 결과의 레퍼런스

	DeviceContext->ClearRenderTargetView(PickingFrameBufferRTV, PickingClearColor);
}

void URenderer::PreparePickingShader() const
{
	DeviceContext->PSSetShader(PickingPixelShader, nullptr, 0);

	if (ConstantPickingBuffer)
	{
		DeviceContext->PSSetConstantBuffers(1, 1, &ConstantPickingBuffer);
	}
}

void URenderer::UpdateConstantPicking(FVector4 UUIDColor) const
{
	if (!ConstantPickingBuffer) return;

	D3D11_MAPPED_SUBRESOURCE ConstantBufferMSR;

	UUIDColor = FVector4(UUIDColor.X / 255.0f, UUIDColor.Y / 255.0f, UUIDColor.Z / 255.0f, UUIDColor.W / 255.0f);

	DeviceContext->Map(ConstantPickingBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ConstantBufferMSR);
	{
		FPickingConstants* Constants = static_cast<FPickingConstants*>(ConstantBufferMSR.pData);
		Constants->UUIDColor = UUIDColor;
	}
	DeviceContext->Unmap(ConstantPickingBuffer, 0);
}

void URenderer::UpdateConstantDepth(int Depth) const
{
	if (!ConstantsDepthBuffer) return;

	ACamera* Cam = FEditorManager::Get().GetCamera();

	D3D11_MAPPED_SUBRESOURCE ConstantBufferMSR;

	DeviceContext->Map(ConstantsDepthBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ConstantBufferMSR);
	{
		FDepthConstants* Constants = static_cast<FDepthConstants*>(ConstantBufferMSR.pData);
		Constants->DepthOffset = Depth;
		Constants->nearPlane = Cam->GetNear();
		Constants->farPlane = Cam->GetFar();
	}
	DeviceContext->Unmap(ConstantsDepthBuffer, 0);
}

void URenderer::PrepareMain()
{
	DeviceContext->OMSetDepthStencilState(DepthStencilState, 0);                // DepthStencil 상태 설정. StencilRef: 스텐실 테스트 결과의 레퍼런스
	DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, DepthStencilView);	// 렌더 타겟 뷰와 깊이-스텐실 뷰를 출력 렌더링 파이프라인에 바인딩
	DeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);               // 블렌딩 상태 설정
}

void URenderer::PrepareMainShader()
{
	DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
	DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
	DeviceContext->IASetInputLayout(SimpleInputLayout);
}

FVector4 URenderer::GetPixel(FVector MPos)
{
	MPos.X = FMath::Clamp(MPos.X, 0.0f, ViewportInfo.Width);
	MPos.Y = FMath::Clamp(MPos.Y, 0.0f, ViewportInfo.Height);
	// 1. Staging 텍스처 생성 (1x1 픽셀)
	D3D11_TEXTURE2D_DESC stagingDesc = {};
	stagingDesc.Width = 1; // 픽셀 1개만 복사
	stagingDesc.Height = 1;
	stagingDesc.MipLevels = 1;
	stagingDesc.ArraySize = 1;
	stagingDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 원본 텍스처 포맷과 동일
	stagingDesc.SampleDesc.Count = 1;
	stagingDesc.Usage = D3D11_USAGE_STAGING;
	stagingDesc.BindFlags = 0;
	stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	ID3D11Texture2D* stagingTexture = nullptr;
	Device->CreateTexture2D(&stagingDesc, nullptr, &stagingTexture);

	// 2. 복사할 영역 정의 (D3D11_BOX)
	D3D11_BOX srcBox = {};
	srcBox.left = static_cast<UINT>(MPos.X);
	srcBox.right = srcBox.left + 1; // 1픽셀 너비
	srcBox.top = static_cast<UINT>(MPos.Y);
	srcBox.bottom = srcBox.top + 1; // 1픽셀 높이
	srcBox.front = 0;
	srcBox.back = 1;
	FVector4 color{ 1, 1, 1, 1 };

	if (stagingTexture == nullptr)
		return color;

	// 3. 특정 좌표만 복사
	DeviceContext->CopySubresourceRegion(
		stagingTexture, // 대상 텍스처
		0,              // 대상 서브리소스
		0, 0, 0,        // 대상 좌표 (x, y, z)
		PickingFrameBuffer, // 원본 텍스처
		0,              // 원본 서브리소스
		&srcBox         // 복사 영역
	);

	// 4. 데이터 매핑
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	DeviceContext->Map(stagingTexture, 0, D3D11_MAP_READ, 0, &mapped);

	// 5. 픽셀 데이터 추출 (1x1 텍스처이므로 offset = 0)
	const BYTE* pixelData = static_cast<const BYTE*>(mapped.pData);

	if (pixelData)
	{
		color.X = static_cast<float>(pixelData[0]); // R
		color.Y = static_cast<float>(pixelData[1]); // G
		color.Z = static_cast<float>(pixelData[2]); // B
		color.W = static_cast<float>(pixelData[3]); // A
	}

	// std::cout << "X: " << (int)color.X << " Y: " << (int)color.Y 
	//           << " Z: " << color.Z << " A: " << color.W << "\n";

	// 6. 매핑 해제 및 정리
	DeviceContext->Unmap(stagingTexture, 0);
	stagingTexture->Release();

	return color;
}

void URenderer::UpdateViewMatrix(const FTransform& CameraTransform)
{
	ViewMatrix = CameraTransform.GetViewMatrix();
}

void URenderer::UpdateProjectionMatrix(ACamera* Camera)
{
	float AspectRatio = UEngine::Get().GetScreenRatio();

	float FOV = FMath::DegreesToRadians(Camera->GetFieldOfView());
	float Near = Camera->GetNear();
	float Far = Camera->GetFar();

	if (Camera->ProjectionMode == ECameraProjectionMode::Perspective)
	{
		ProjectionMatrix = FMatrix::PerspectiveFovLH(FOV, AspectRatio, Near, Far);
	}
	else if (Camera->ProjectionMode == ECameraProjectionMode::Orthographic)
	{
		ProjectionMatrix = FMatrix::OrthoLH(UEngine::Get().GetScreenWidth(),
			UEngine::Get().GetScreenHeight(), Near, Far);

		// TODO: 추가 필요.
		// ProjectionMatrix = FMatrix::OrthoForLH(FOV, AspectRatio, Near, Far);
	}
}

void URenderer::OnUpdateWindowSize(int Width, int Height)
{
	if (SwapChain)
	{
		ReleaseFrameBuffer();
		ReleasePickingFrameBuffer();
		ReleaseDepthStencilBuffer();

		SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0);

		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		SwapChain->GetDesc(&SwapChainDesc);
		// 뷰포트 정보 갱신
		ViewportInfo = {
			0.0f, 0.0f,
			static_cast<float>(SwapChainDesc.BufferDesc.Width), static_cast<float>(SwapChainDesc.BufferDesc.Height),
			0.0f, 1.0f
		};

		// 프레임 버퍼를 다시 생성
		CreateFrameBuffer();

		CreatePickingTexture(UEngine::Get().GetWindowHandle());

		// 뎁스 스텐실 버퍼를 다시 생성
		CreateDepthStencilBuffer();
	}
}

void URenderer::RenderPickingTexture()
{
	// 백버퍼로 복사
	ID3D11Texture2D* backBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	DeviceContext->CopyResource(backBuffer, PickingFrameBuffer);
	backBuffer->Release();
}

void URenderer::TurnZBufferOn()
{
	DeviceContext->OMSetDepthStencilState(DepthStencilState, 1);
}

void URenderer::TurnZBufferOff()
{
	DeviceContext->OMSetDepthStencilState(IgnoreDepthStencilState, 1);
}


void URenderer::CreateAlphaBlendingState()
{
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// 알파 블렌딩을 위한 설정
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// 알파블렌딩 on
	Device->CreateBlendState(&blendStateDescription, &AlphaEnableBlendingState);

	// 파티클 알파블렌딩 설정
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	Device->CreateBlendState(&blendStateDescription, &ParticleAlphaEnableBlendingState);


	// 알파블렌딩 끈 상태도 생성
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	Device->CreateBlendState(&blendStateDescription, &AlphaDisableBlendingState);
}

void URenderer::ReleaseAlphaBlendingState()
{
	if (AlphaEnableBlendingState)
	{
		AlphaEnableBlendingState->Release();
		AlphaEnableBlendingState = nullptr;
	}
	if (ParticleAlphaEnableBlendingState)
	{
		ParticleAlphaEnableBlendingState->Release();
		ParticleAlphaEnableBlendingState = nullptr;
	}
	if (AlphaDisableBlendingState)
	{
		AlphaDisableBlendingState->Release();
		AlphaDisableBlendingState = nullptr;
	}
}

void URenderer::TurnOnAlphaBlending(EAlphaBlendingState State)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	switch (State)
	{
	case EAlphaBlendingState::Normal:
		DeviceContext->OMSetBlendState(AlphaEnableBlendingState, blendFactor, 0xffffffff);
		break;

	case EAlphaBlendingState::Particle:
		DeviceContext->OMSetBlendState(ParticleAlphaEnableBlendingState, blendFactor, 0xffffffff);
		break;
	}
}

void URenderer::TurnOffAlphaBlending()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	DeviceContext->OMSetBlendState(AlphaDisableBlendingState, blendFactor, 0xffffffff);
}

void URenderer::CreateText(HWND hWindow)
{
	Text = new UText();
	Text->Create(Device, DeviceContext, hWindow, UEngine::Get().GetScreenWidth(), UEngine::Get().GetScreenHeight());
}

void URenderer::RenderText(const FString& InText, const FVector& InTextPos, const FVector& InTextSize)
{
	// 텍스트의 WorldMatrix 결정
	ACamera* Camera = FEditorManager::Get().GetCamera();
	// 스케일 * 회전
	FMatrix WorldMatrix = FMatrix::GetScaleMatrix(InTextSize) *
		FMatrix::GetRotateMatrix(FQuat(Camera->GetActorRelativeTransform().GetRotation())).Inverse();
	// 이동은 스케일의 영향을 받지 않게 따로
	WorldMatrix.SetTranslateMatrix(InTextPos);


	// 텍스트 렌더링
	DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	TurnOnAlphaBlending(EAlphaBlendingState::Normal);
	TurnZBufferOff();

	Text->Render(DeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, InText);

	TurnOffAlphaBlending();
	TurnZBufferOn();
}


void URenderer::CreateParticle(HWND hWindow)
{
	UParticle::Get().Create(Device, DeviceContext, L"Source/Core/Rendering/Particle/particle_config_01.txt");

	ParticleShader = new UParticleShader();
	ParticleShader->Create(Device, hWindow);
}

void URenderer::RenderParticle(float DeltaTime)
{
	DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	CurrentTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	DeviceContext->IASetPrimitiveTopology(CurrentTopology);

	// 텍스트의 WorldMatrix 결정
	ACamera* Camera = FEditorManager::Get().GetCamera();
	// 스케일 * 회전
	FMatrix WorldMatrix = FMatrix::GetScaleMatrix(1) *
		FMatrix::GetRotateMatrix(FQuat(Camera->GetActorRelativeTransform().GetRotation())).Inverse();
	// 이동은 스케일의 영향을 받지 않게 따로
	WorldMatrix.SetTranslateMatrix(0);

	TurnOnAlphaBlending(EAlphaBlendingState::Particle);
	TurnZBufferOff();

	UParticle::Get().Frame(DeltaTime, DeviceContext);
	UParticle::Get().Render(DeviceContext);
	ParticleShader->Render(DeviceContext, UParticle::Get().GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, UParticle::Get().GetTexture());

	TurnOffAlphaBlending();
	TurnZBufferOn();
}

void URenderer::CreateTexture(HWND hWindow)
{
	Texture = new UTexture();
	TextureRenderer = new UTextureRenderer();
	TextureRenderer->Create(Device, hWindow);
	Texture->Create(Device, L"light_bulb_texture.tga");
}

void URenderer::RenderTexture(const FVector& InPos)
{
	DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	CurrentTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	DeviceContext->IASetPrimitiveTopology(CurrentTopology);

	// 텍스트의 WorldMatrix 결정
	ACamera* Camera = FEditorManager::Get().GetCamera();
	// 스케일 * 회전
	FMatrix WorldMatrix = FMatrix::GetScaleMatrix(1) *
		FMatrix::GetRotateMatrix(FQuat(Camera->GetActorRelativeTransform().GetRotation())).Inverse();
	// 이동은 스케일의 영향을 받지 않게 따로
	WorldMatrix.SetTranslateMatrix(InPos);

	TurnOnAlphaBlending(EAlphaBlendingState::Normal);
	TurnZBufferOff();

	Texture->Render(Device, DeviceContext);
	TextureRenderer->Render(DeviceContext, 6, WorldMatrix, ViewMatrix, ProjectionMatrix, Texture->GetTexture());

	TurnOffAlphaBlending();
	TurnZBufferOn();
}

FVector2 URenderer::GetSwapChainSize()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	SwapChain->GetDesc(&swapChainDesc);

	float fullWidth = static_cast<float>(swapChainDesc.BufferDesc.Width);
	float fullHeight = static_cast<float>(swapChainDesc.BufferDesc.Height);

	return FVector2(fullWidth,fullHeight);
}


void URenderer::SetViewMode(EViewModeIndex viewMode)
{
	CurrentViewMode = viewMode;

	switch (CurrentViewMode)
	{
	case EViewModeIndex::VMI_Lit:
	case EViewModeIndex::VMI_Unlit:
		CurrentFillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		break;
	case EViewModeIndex::VMI_Wireframe:
		CurrentFillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		break;
	}
	CreateRasterizerState();
}

EViewModeIndex URenderer::GetCurrentViewMode() const
{
	return CurrentViewMode;

}