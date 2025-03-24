#pragma once
#include "Object/Actor/Actor.h"
#include "Data/FEngineShowFlags.h"

class AGizmoHandle;
class AWorldGrid;

namespace ECameraViewMode{  enum class Type : uint8;  }

class FEditorManager : public TSingleton<FEditorManager>
{
public:
    
    inline AActor* GetSelectedActor() const {return SelectedActor;}

    inline UPrimitiveComponent* GetSelectedComponent() const {if (SelectedComponent)    {return SelectedComponent;}
                                                            else                        {return nullptr;}}
    
    void SelectPrimitive(UPrimitiveComponent* NewPrimitive);
    void SelectActor(AActor* NewActor);

    inline ACamera* GetCamera() const {return Camera;}

    void SetCamera(ACamera* NewCamera);
    void SetOrthoCamera(ECameraViewMode::Type type);

    void AddOrthoCamera(ECameraViewMode::Type type, ACamera* NewMultiCamera);

    AGizmoHandle* GetGizmoHandle() const {return GizmoHandle;}

    void SetWorldGrid(AWorldGrid* NewWorldGrid) { WorldGrid = NewWorldGrid; };

    AWorldGrid* GetWorldGrid() const {return WorldGrid;};
    
    bool IsShowFlagSet(EEngineShowFlags Flag) const { return EngineShowFlags.IsSet(Flag);}
    void SetShowFlag(EEngineShowFlags Flag, bool bEnabled) { EngineShowFlags.Set(Flag, bEnabled);}
    void ToggleShowFlag(EEngineShowFlags Flag) { EngineShowFlags.Toggle(Flag); }
    void PrintShowFlags() const { EngineShowFlags.Print(); }
    
    void MoveAllOrthoCameras(ECameraViewMode::Type cameraType, FVector MouseDeltaPos);
    void MoveOrthoCamera(ECameraViewMode::Type cameraType, FVector deltaMove);
private:
    AActor* SelectedActor = nullptr;
    UPrimitiveComponent* SelectedComponent = nullptr;
    
    ACamera* Camera = nullptr;
    TMap<ECameraViewMode::Type,ACamera*> OrthogonalCamera;
    AGizmoHandle* GizmoHandle = nullptr;
    AWorldGrid* WorldGrid = nullptr;
    FEngineShowFlags EngineShowFlags;

};
