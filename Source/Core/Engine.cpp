#include "Engine.h"

#include <iostream>
#include "Object/ObjectFactory.h"
#include "Object/World/World.h"
#include "Debug/DebugConsole.h"
#include "Object/Gizmo/Axis.h"
#include "Core/Input/PlayerInput.h"
#include "Core/Input/PlayerController.h"
#include "Object/Actor/Camera.h"
#include "Object/Actor/Sphere.h"
#include "Object/Actor/WorldGrid.h"
#include "Static/FEditorManager.h"
#include"Data/ConfigManager.h"
#include "../FSlateApplication.h"

class AArrow;
class APicker;
// ImGui WndProc 정의
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT UEngine::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // ImGui의 메시지를 처리
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
    {
        return true;
    }
    
    switch (uMsg)
    {
        // 창이 제거될 때 (창 닫기, Alt+F4 등)
    case WM_DESTROY:
        PostQuitMessage(0); // 프로그램 종료
        break;
    case WM_KEYDOWN:
        APlayerInput::Get().KeyDown(static_cast<EKeyCode>(wParam));
        if ((lParam>>30)%2 != 0)
        {
            APlayerInput::Get().KeyOnceUp(static_cast<EKeyCode>( wParam ));
        }
        break;
    case WM_KEYUP:
        APlayerInput::Get().KeyUp(static_cast<EKeyCode>( wParam ));
        break;
    case WM_LBUTTONDOWN:
        APlayerInput::Get().HandleMouseInput(hWnd, lParam, true, false);
        break;
    case WM_LBUTTONUP:
        APlayerInput::Get().HandleMouseInput(hWnd, lParam, false, false);
        break;
    case WM_RBUTTONDOWN:
        APlayerInput::Get().HandleMouseInput(hWnd, lParam, true, true);
        break;
    case WM_RBUTTONUP:
        APlayerInput::Get().HandleMouseInput(hWnd, lParam, false, true);
        break;
    case WM_SIZE:
		UEngine::Get().UpdateWindowSize(LOWORD(lParam), HIWORD(lParam));
		break;        
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

void UEngine::Initialize(
    HINSTANCE hInstance, const WCHAR* InWindowTitle, const WCHAR* InWindowClassName, int InScreenWidth,
    int InScreenHeight,
    EScreenMode InScreenMode
)
{
    WindowInstance = hInstance;
    WindowTitle = InWindowTitle;
    WindowClassName = InWindowClassName;
    ScreenMode = InScreenMode;
    ScreenWidth = InScreenWidth;
    ScreenHeight = InScreenHeight;

    InitWindow(InScreenWidth, InScreenWidth);
    InitRenderer();

    InitWorld();

    InitializedScreenWidth = ScreenWidth;
    InitializedScreenHeight = ScreenHeight;
    
    ui.Initialize(WindowHandle, *Renderer, ScreenWidth, ScreenHeight);
    FSlateApplication::Get().Initialize();
	UE_LOG("Engine Initialized!");
}

void UEngine::Run()
{
    // FPS 제한
    constexpr int TargetFPS = 750;
    constexpr double TargetDeltaTime = 1000.0f / TargetFPS; // 1 FPS의 목표 시간 (ms)

    // 고성능 타이머 초기화
    LARGE_INTEGER Frequency;
    QueryPerformanceFrequency(&Frequency);

    LARGE_INTEGER StartTime;
    QueryPerformanceCounter(&StartTime);
    
    IsRunning = true;
    while (IsRunning)
    {
        // DeltaTime 계산 (초 단위)
        const LARGE_INTEGER EndTime = StartTime;
        QueryPerformanceCounter(&StartTime);

        const float DeltaTime =
            static_cast<float>(StartTime.QuadPart - EndTime.QuadPart) / static_cast<float>(Frequency.QuadPart);

        APlayerInput::Get().PreProcessInput();
        
        // 메시지(이벤트) 처리
        MSG Msg;
        while (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
        {
            // 키 입력 메시지를 번역
            TranslateMessage(&Msg);

            // 메시지를 등록한 Proc에 전달
            DispatchMessage(&Msg);

            if (Msg.message == WM_QUIT)
            {
                IsRunning = false;
                break;
            }

        }

        if (World == nullptr)
        {
            continue;
        }
        
		// Renderer Update
        Renderer->Prepare();
        Renderer->PrepareShader();
        
		// World Update
		if (World)
		{
			World->Tick(DeltaTime);
			World->Render(DeltaTime);
		    World->LateTick(DeltaTime);
		}

        //각 Actor에서 TickActor() -> PlayerTick() -> TickPlayerInput() 호출하는데 지금은 Message에서 처리하고 있다.
        APlayerInput::Get().TickPlayerInput(); //잘못된 위치. 위에 달린 주석대로 처리해야 정상 플레이어 액터 내에서만 처리해야할것같다.
        
        // TickPlayerInput
        FSlateApplication::Get().Tick();
        APlayerController::Get().ProcessPlayerInput(DeltaTime);
        
		// ui Update
        ui.Update();

        Renderer->SwapBuffer();

        // FPS 제한
        double ElapsedTime;
        do
        {
            Sleep(0);

            LARGE_INTEGER CurrentTime;
            QueryPerformanceCounter(&CurrentTime);

            ElapsedTime = static_cast<double>(CurrentTime.QuadPart - StartTime.QuadPart) * 1000.0 / static_cast<double>(Frequency.QuadPart);
        } while (ElapsedTime < TargetDeltaTime);
    }
}


void UEngine::Shutdown()
{
    Renderer->Release();
    
    ConfigManager::Get().SaveAllConfigs();

#ifdef _DEBUG
    World->SaveWorld(*World->DebugDefaultSceneName);
#else
    World->SaveWorld(*World->DefaultSceneName)
#endif
    FSlateApplication::Get().ShutDown();
    ShutdownWindow();
}


void UEngine::InitWindow(int InScreenWidth, int InScreenHeight)
{
    // 윈도우 클래스 등록
    WNDCLASSW wnd_class{};
    wnd_class.lpfnWndProc = WndProc;
    wnd_class.hInstance = WindowInstance;
    wnd_class.lpszClassName = WindowClassName;
    RegisterClassW(&wnd_class);

    // Window Handle 생성
    WindowHandle = CreateWindowExW(
        WS_EX_NOREDIRECTIONBITMAP, WindowClassName, WindowTitle,
        WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        InScreenWidth, InScreenHeight,
        nullptr, nullptr, WindowInstance, nullptr
    );

    // TODO: 전체화면 구현
    if (ScreenMode != EScreenMode::Windowed)
    {
        std::cout << "not implement Fullscreen and Borderless mode." << '\n';
    }

    // 윈도우 포커싱
    ShowWindow(WindowHandle, SW_SHOW);
    SetForegroundWindow(WindowHandle);
    SetFocus(WindowHandle);

    //AllocConsole(); // 콘솔 창 생성

    //// 표준 출력 및 입력을 콘솔과 연결
    //freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    //freopen_s((FILE**)stdin, "CONIN$", "r", stdin);

    //std::cout << "Debug Console Opened!" << '\n';
}

void UEngine::InitRenderer()
{
	// 렌더러 초기화
	Renderer = std::make_unique<URenderer>();
	Renderer->Create(WindowHandle);
	Renderer->CreateShader();
	Renderer->CreateConstantBuffer();
}

void UEngine::InitWorld()
{
    World = FObjectFactory::ConstructObject<UWorld>();
    
    
    ACamera* mainCamera = World->SpawnActor<ACamera>();
    FEditorManager::Get().AddCamera(mainCamera);
    FEditorManager::Get().SetCamera(mainCamera);        //메인 카메라 설정

    float d = 10.0f;

    //Front
    ACamera* frontCamera = World->SpawnActor<ACamera>();
    frontCamera->ProjectionMode = ECameraProjectionMode::Type::Perspective;
    frontCamera->SetCameraViewMode(ECameraViewMode::Type::Front);
    FTransform frontTransform = frontCamera->GetActorRelativeTransform();
    frontTransform.SetPosition(FVector(d,0,0));
    frontCamera->SetActorRelatvieTransform(frontTransform);
    FEditorManager::Get().AddCamera(frontCamera);
    FEditorManager::Get().AddOrthoCamera(ECameraViewMode::Type::Front,frontCamera);

    //Top
    ACamera* topCamera = World->SpawnActor<ACamera>();
    topCamera->ProjectionMode = ECameraProjectionMode::Type::Perspective;
    topCamera->SetCameraViewMode(ECameraViewMode::Type::Top);
    FTransform topTransform = topCamera->GetActorRelativeTransform();
    topTransform.SetPosition(FVector(0, 0, d));
    topCamera->SetActorRelatvieTransform(topTransform);
    FEditorManager::Get().AddCamera(topCamera);
    FEditorManager::Get().AddOrthoCamera(ECameraViewMode::Type::Top,topCamera);

    //Right
    ACamera* rightCamera = World->SpawnActor<ACamera>();
    rightCamera->ProjectionMode = ECameraProjectionMode::Type::Perspective;
    rightCamera->SetCameraViewMode(ECameraViewMode::Type::Right);
    FTransform rightTransform = rightCamera->GetActorRelativeTransform();
    rightTransform.SetPosition(FVector(0, d, 0));
    rightCamera->SetActorRelatvieTransform(rightTransform);
    FEditorManager::Get().AddCamera(rightCamera);
    FEditorManager::Get().AddOrthoCamera(ECameraViewMode::Type::Right,rightCamera);


    //Back
    ACamera* backCamera = World->SpawnActor<ACamera>();
    backCamera->ProjectionMode = ECameraProjectionMode::Type::Perspective;
    backCamera->SetCameraViewMode(ECameraViewMode::Type::Back);
    FTransform backTransform = backCamera->GetActorRelativeTransform();
    backTransform.SetPosition(FVector(-d, 0, 0));
    backCamera->SetActorRelatvieTransform(backTransform);
    //FEditorManager::Get().AddCamera(backCamera);
    FEditorManager::Get().AddOrthoCamera(ECameraViewMode::Type::Back,backCamera);

    //Bottom
    ACamera* bottomCamera = World->SpawnActor<ACamera>();
    bottomCamera->ProjectionMode = ECameraProjectionMode::Type::Perspective;
    bottomCamera->SetCameraViewMode(ECameraViewMode::Type::Bottom);
    FTransform bottomTransform = bottomCamera->GetActorRelativeTransform();
    bottomTransform.SetPosition(FVector(0, 0, -d));
    bottomCamera->SetActorRelatvieTransform(bottomTransform);
    //FEditorManager::Get().AddCamera(bottomCamera);
    FEditorManager::Get().AddOrthoCamera(ECameraViewMode::Type::Bottom,bottomCamera);

    //Left
    ACamera* leftCamera = World->SpawnActor<ACamera>();
    leftCamera->ProjectionMode = ECameraProjectionMode::Type::Perspective;
    leftCamera->SetCameraViewMode(ECameraViewMode::Type::Left);
    FTransform leftTransform = leftCamera->GetActorRelativeTransform();
    leftTransform.SetPosition(FVector(0, -d, 0));
    leftCamera->SetActorRelatvieTransform(leftTransform);
    //FEditorManager::Get().AddCamera(leftCamera);
    FEditorManager::Get().AddOrthoCamera(ECameraViewMode::Type::Left,leftCamera);


    FEditorManager::Get().SetWorldGrid(World->SpawnActor<AWorldGrid>());

    ConfigManager::Get().LoadAllConfigs();
#ifdef _DEBUG
    World->LoadWorld(*World->DebugDefaultSceneName);
#else
    World->LoadWorld(*World->ReleaseDefaultSceneName);
#endif

    //// Test
    // World->SpawnActor<AArrow>();
    // World->SpawnActor<ASphere>();
    
    World->SpawnActor<AAxis>();
    World->SpawnActor<APicker>();

	World->BeginPlay();
}

void UEngine::ShutdownWindow()
{
    DestroyWindow(WindowHandle);
    WindowHandle = nullptr;

    UnregisterClassW(WindowClassName, WindowInstance);
    WindowInstance = nullptr;

	ui.Shutdown();
}

void UEngine::UpdateWindowSize(UINT InScreenWidth, UINT InScreenHeight)
{
	ScreenWidth = InScreenWidth;
	ScreenHeight = InScreenHeight;

    if(Renderer)
    {
        Renderer->OnUpdateWindowSize(ScreenWidth, ScreenHeight);
    }

	if (ui.bIsInitialized)
	{
		ui.OnUpdateWindowSize(ScreenWidth, ScreenHeight);
	}
}

UObject* UEngine::GetObjectByUUID(uint32 InUUID) const
{
    if (const auto Obj = GObjects.Find(InUUID))
    {
        return Obj->get();
    }
    return nullptr;
}
