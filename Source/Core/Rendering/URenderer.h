#pragma once

#define _TCHAR_DEFINED
#include <d3d11.h>
#include <memory>

#include "UI.h"
#include "Core/Math/Vector.h"
// #include "Object/Actor/Camera.h"
#include "Core/Math/Matrix.h"
#include "Core/Engine.h"
#include "Primitive/PrimitiveVertices.h"
#include "Core/Math/Plane.h"
#include "Core/Math/Transform.h"
#include "Object/USceneComponent.h"
#include "Text/Text.h"


struct FVertexSimple;
struct FVector4;
class UPrimitiveComponent;
class ACamera;

struct VertexBufferInfo
{
public:
	VertexBufferInfo() = default;
	VertexBufferInfo(ID3D11Buffer* InVertexBuffer, ID3D11Buffer* InIndexBuffer, D3D_PRIMITIVE_TOPOLOGY InTopology, TArray<FVertexSimple>& InVertices, TArray<uint32_t>& InIndices)
	{
		VertexBuffer = InVertexBuffer;
		IndexBuffer = InIndexBuffer;
		
		VertexCount = InVertices.Num();
		IndexCount = InIndices.Num();
		
		Topology = InTopology;
		//orginVertex에서 가져올거라서 복사해와야함
		Vertices = InVertices;
		Indices = InIndices;
		
		PreVertexCount = VertexCount;
	}

	VertexBufferInfo(D3D_PRIMITIVE_TOPOLOGY InTopology, TArray<FVertexSimple>& InVertices, TArray<uint32_t>& InIndices)
	{
		// InstanceBuffer = InInstanceBuffer;
		VertexCount = InVertices.Num();
		IndexCount = InIndices.Num();
		Topology = InTopology;
		//orginVertex에서 가져올거라서 복사해와야함
		Vertices = InVertices;
		Indices = InIndices;
		PreVertexCount = VertexCount;
	}
	
	~VertexBufferInfo()
	{
	}

	ID3D11Buffer*& GetVertexBuffer() { return VertexBuffer; }
	uint32_t GetVertexCount() const { return VertexCount; }
	ID3D11Buffer*& GetIndexBuffer() { return IndexBuffer; }
	uint32_t GetIndexCount() const{return IndexCount;}
	
	D3D_PRIMITIVE_TOPOLOGY GetTopology() const { return Topology; }
	TArray<FVertexSimple> GetVertices() const { return Vertices; }
	TArray<uint32_t> GetIndices() const { return Indices; }
	uint32_t GetVertexPreCount() const { return PreVertexCount; }

	void AddVertices(TArray<FVertexSimple> InVertices, TArray<uint32_t> InIndices);
	void ClearVertices();
	
private:
	ID3D11Buffer* VertexBuffer;
	TArray<FVertexSimple> Vertices;
	uint32_t VertexCount;
	uint32_t PreVertexCount;

	ID3D11Buffer* IndexBuffer;
	TArray<uint32_t> Indices;
	uint32_t IndexCount;
	
	D3D_PRIMITIVE_TOPOLOGY Topology;
};

enum class EViewModeIndex : uint32
{
    VMI_Lit,
    VMI_Unlit,
    VMI_Wireframe,
};

class URenderer
{
private:
    struct alignas(16) FConstants
    {
        FMatrix M;
    	FMatrix V;
    	FMatrix P;
        // FVector4 Color;
		// true인 경우 Vertex Color를 사용하고, false인 경우 Color를 사용합니다.
        // uint32 bUseVertexColor;
    };
	
	struct alignas(16) FPickingConstants
	{
		FVector4 UUIDColor;
	};

	struct alignas(16) FDepthConstants{
		unsigned int DepthOffset;
		int nearPlane;
		int farPlane;
	};
	
    struct ConstantUpdateInfo
    {
        // transformation에 관련된 정보는 FTransform이 아니라 FMatrix으로 넘깁니다
        // FTransform은 RelativeLocation, RelativeRotation, RelativeScale3D의 컨테이너로 취급해야합니다.
        // FMatrix -> FTransform의 변경은 유효한지 확인되지 않았습니다.
        const FMatrix& TransformMatrix; 
		const FVector4& Color;
        bool bUseVertexColor;
    };

public:
    ~URenderer();

    /** Renderer를 초기화 합니다. */
    void Create(HWND hWindow);

    /** Renderer에 사용된 모든 리소스를 해제합니다. */
    void Release();

    void CreateShader();

    void ReleaseShader();

    void CreateConstantBuffer();

    void ReleaseConstantBuffer();

    /** 스왑 체인의 백 버퍼와 프론트 버퍼를 교체하여 화면에 출력 */
    void SwapBuffer() const;

    /** 렌더링 파이프라인을 준비 합니다. */
    void Prepare();

    /** 셰이더를 준비 합니다. */
    void PrepareShader() const;
    void RenderBatch();

    void CreateVertexBuffer(EPrimitiveType VertexType, VertexBufferInfo BufferInfo);
    void CreateBatchVertexBuffer(D3D11_PRIMITIVE_TOPOLOGY Topology, VertexBufferInfo BufferInfo);
    void ClearBatchVertex();
	void AddBatchVertices(UPrimitiveComponent* Component);
    VertexBufferInfo ResizeBatchVertexBuffer(D3D11_PRIMITIVE_TOPOLOGY Topology);
    void UpdateBatchVertexBuffer();
    TMap<D3D11_PRIMITIVE_TOPOLOGY, bool> CheckChangedVertexCount();

	VertexBufferInfo GetBatchVertexBufferInfo(D3D11_PRIMITIVE_TOPOLOGY Topology) { return BatchVertexBuffers[Topology];}
	void SetBatchVertexBufferInfo(D3D11_PRIMITIVE_TOPOLOGY Topology, VertexBufferInfo BufferInfo)
	{
		if (BatchVertexBuffers.Contains(Topology)){ BatchVertexBuffers[Topology] = BufferInfo;}
		else							{    BatchVertexBuffers.Add(Topology, BufferInfo);}
	}

	VertexBufferInfo GetVertexBufferInfo(EPrimitiveType VertexType) { return VertexBuffers[VertexType];}
	void SetVertexBufferInfo(EPrimitiveType VertexType, VertexBufferInfo BufferInfo)
	{
		if (VertexBuffers.Contains(VertexType)){ VertexBuffers[VertexType] = BufferInfo; }
		else									{ VertexBuffers.Add(VertexType, BufferInfo); }
	}
	
    /** Buffer를 해제합니다. */
	void ReleaseVertexBuffer(D3D11_PRIMITIVE_TOPOLOGY Topology);
    void RenderPrimtivie(UPrimitiveComponent* Component);
    void ReleaseAllVertexBuffer();
    /** Constant Data를 업데이트 합니다. */
    void UpdateConstant(USceneComponent* Component) const;

    ID3D11Device* GetDevice() const;
    ID3D11DeviceContext* GetDeviceContext() const;
	
    /** View 변환 Matrix를 업데이트 합니다. */
    void UpdateViewMatrix(const FTransform& CameraTransform);

    /** Projection 변환 Matrix를 업데이트 합니다. */
    void UpdateProjectionMatrix(ACamera* Camera);

	void OnUpdateWindowSize(int Width, int Height);

    /** Alpha Blending을 켜고 끄는 함수 **/
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

    void CreateText(HWND hWindow);

    void RenderText(const FString& InText, const FVector& InTextPos, const FVector& InTextSize);

    void CreateAlphaBlendingState();
	void ReleaseAlphaBlendingState();

    void TurnZBufferOn();
    void TurnZBufferOff();

    void SetViewMode(EViewModeIndex viewMode);

    EViewModeIndex GetCurrentViewMode() const;

protected:
    /** Direct3D Device 및 SwapChain을 생성합니다. */
    void CreateDeviceAndSwapChain(HWND hWindow);

    /** Direct3D Device 및 SwapChain을 해제합니다.  */
    void ReleaseDeviceAndSwapChain();

    /** 프레임 버퍼를 생성합니다. */
    void CreateFrameBuffer();

    /** 뎁스 스텐실 버퍼를 생성합니다. */
	void CreateDepthStencilBuffer();

    /** 뎁스 스텐실 상태를 생성합니다. */
	void CreateDepthStencilState();
	
    /** 프레임 버퍼를 해제합니다. */
    void ReleaseFrameBuffer();

	/** 뎁스 스텐실 버퍼를 해제합니다. */
	void ReleaseDepthStencilBuffer();
	
    /** 레스터라이즈 상태를 생성합니다. */
    void CreateRasterizerState();

    /** 레스터라이저 상태를 해제합니다. */
    void ReleaseRasterizerState();

    // void CreateBufferCache();

    void InitMatrix();

protected:
    // Direct3D 11 장치(Device)와 장치 컨텍스트(Device Context) 및 스왑 체인(Swap Chain)을 관리하기 위한 포인터들
    ID3D11Device* Device = nullptr;                         // GPU와 통신하기 위한 Direct3D 장치
    ID3D11DeviceContext* DeviceContext = nullptr;           // GPU 명령 실행을 담당하는 컨텍스트
    IDXGISwapChain* SwapChain = nullptr;                    // 프레임 버퍼를 교체하는 데 사용되는 스왑 체인

    // 렌더링에 필요한 리소스 및 상태를 관리하기 위한 변수들
    ID3D11Texture2D* FrameBuffer = nullptr;                 // 화면 출력용 텍스처
    ID3D11RenderTargetView* FrameBufferRTV = nullptr;       // 텍스처를 렌더 타겟으로 사용하는 뷰
    ID3D11RasterizerState* RasterizerState = nullptr;       // 래스터라이저 상태(컬링, 채우기 모드 등 정의)
    ID3D11Buffer* ConstantBuffer = nullptr;                 // 쉐이더에 데이터를 전달하기 위한 상수 버퍼
	TMap<D3D11_PRIMITIVE_TOPOLOGY, VertexBufferInfo> BatchVertexBuffers;
	TMap<EPrimitiveType, VertexBufferInfo> VertexBuffers;
	
    FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f }; // 화면을 초기화(clear)할 때 사용할 색상 (RGBA)
    D3D11_VIEWPORT ViewportInfo = {};                       // 렌더링 영역을 정의하는 뷰포트 정보

    // Shader를 렌더링할 때 사용되는 변수들
    ID3D11VertexShader* SimpleVertexShader = nullptr;       // Vertex 데이터를 처리하는 Vertex 셰이더
    ID3D11PixelShader* SimplePixelShader = nullptr;         // Pixel의 색상을 결정하는 Pixel 셰이더

    ID3D11InputLayout* SimpleInputLayout = nullptr;         // Vertex 셰이더 입력 레이아웃 정의
    unsigned int Stride = 0;                                // Vertex 버퍼의 각 요소 크기

    // Depth Stenil Buffer
	ID3D11Texture2D* DepthStencilBuffer = nullptr;          // DepthStencil버퍼 역할을 하는 텍스쳐
	ID3D11DepthStencilView* DepthStencilView = nullptr;     // DepthStencil버퍼를 렌더 타겟으로 사용하는 뷰
	ID3D11DepthStencilState* DepthStencilState = nullptr;   // DepthStencil 상태(깊이 테스트, 스텐실 테스트 등 정의)
    ID3D11DepthStencilState* GizmoDepthStencilState = nullptr; // 기즈모용 스텐실 스테이트. Z버퍼 테스트 하지않고 항상 앞에렌더
	
	// Buffer Cache

	// std::unique_ptr<FBufferCache> BufferCache;

	FMatrix WorldMatrix;
    FMatrix ViewMatrix;
	FMatrix ProjectionMatrix;
	
	D3D11_PRIMITIVE_TOPOLOGY CurrentTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED; // 같은 토폴로지 세팅인데 또 하면 낭비니까 체크


	// Alpha Blending
	ID3D11BlendState* AlphaEnableBlendingState = nullptr;
	ID3D11BlendState* AlphaDisableBlendingState = nullptr;

    // 텍스트클래스
    UText* Text = nullptr;

    EViewModeIndex CurrentViewMode = EViewModeIndex::VMI_Lit;
    D3D11_FILL_MODE CurrentFillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

#pragma region picking
protected:
	// 피킹용 버퍼들
	ID3D11Texture2D* PickingFrameBuffer = nullptr;                 // 화면 출력용 텍스처
	ID3D11RenderTargetView* PickingFrameBufferRTV = nullptr;       // 텍스처를 렌더 타겟으로 사용하는 뷰
	ID3D11Buffer* ConstantPickingBuffer = nullptr;                 // 뷰 상수 버퍼
	FLOAT PickingClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //
	ID3D11PixelShader* PickingPixelShader = nullptr;         // Pixel의 색상을 결정하는 Pixel 셰이더
	ID3D11Buffer* ConstantsDepthBuffer = nullptr;

	ID3D11DepthStencilState* IgnoreDepthStencilState = nullptr;   // DepthStencil 상태(깊이 테스트, 스텐실 테스트 등 정의)

public:
	//피킹용 함수들	
    void ReleasePickingFrameBuffer();
    void CreatePickingTexture(HWND hWnd);
    void PrepareZIgnore();
    void PreparePicking();
	void PreparePickingShader() const;
	void UpdateConstantPicking(FVector4 UUIDColor) const;
    void UpdateConstantDepth(int Depth) const;

    void PrepareMain();
	void PrepareMainShader();

	FVector4 GetPixel(FVector MPos);

	void RenderPickingTexture();

    void CreateConeVertices();
    void CreateCylinderVertices();
    FMatrix GetProjectionMatrix() const { return ProjectionMatrix; }

#pragma endregion picking
};
