#pragma once
#include "Data/UDeveloperSettings.h"
#include "Core/Container/String.h"

class ACamera;

class UCameraSettings : public UDeveloperSettings
{
public:
	UCameraSettings(const FString& InConfigFilename);
	~UCameraSettings();

protected:
	ACamera* Camera;

	virtual bool LoadConfig() override;
	virtual bool SaveConfig() override;
};