#pragma once
#include "Data/UDeveloperSettings.h"
#include "Core/Container/String.h"
#include "Object/Actor/WorldGrid.h"

class ACamera;

class USettingsLoader : public UDeveloperSettings
{
public:
	USettingsLoader(const FString& InConfigFilename);
	~USettingsLoader();

protected:
	ACamera* Camera;
	AWorldGrid* WorldGrid;
	
	virtual bool LoadConfig() override;
	virtual bool SaveConfig() override;
};