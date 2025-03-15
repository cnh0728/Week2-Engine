#include "UCameraSettings.h"
#include "Object/World/World.h"
#include "Static/FEditorManager.h"
#include "Object/Actor/Camera.h"
#include <fstream>

UCameraSettings::UCameraSettings(const FString& InConfigFilename)
	:UDeveloperSettings(InConfigFilename)
{
	Camera = FEditorManager::Get().GetCamera();
}

bool UCameraSettings::LoadConfig()
{
	if (!UDeveloperSettings::LoadConfig())
		return false;

	if (ConfigData.find(FString("CameraSensitivity")) != ConfigData.end())
	{
		try {
			float Sensitivity = std::stof(std::string(*(ConfigData[FString("CameraSensitivity")])));
			Camera->SetCameraSensitivity(Sensitivity);
		}
		catch (...)
		{
			Camera->SetCameraSensitivity(1.0f);
		}
	}

	return true;
}

bool UCameraSettings::SaveConfig()
{
	ConfigData[FString("CameraSensitivity")] = FString(std::to_string(Camera->GetCameraSensitivity()));

	std::ofstream OutFile(*ConfigFilename);
	if (!OutFile.is_open())
		return false;

	for (const auto& Pair : ConfigData)
	{
		OutFile << *Pair.first << "=" << *Pair.second << "\n";
	}

	OutFile.close();
	return true;
}