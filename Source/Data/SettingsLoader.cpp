#include "SettingsLoader.h"
#include "Object/World/World.h"
#include "Static/FEditorManager.h"
#include "Object/Actor/Camera.h"
#include <fstream>

USettingsLoader::USettingsLoader(const FString& InConfigFilename)
	:UDeveloperSettings(InConfigFilename)
{
	Camera = FEditorManager::Get().GetCamera();
	WorldGrid = FEditorManager::Get().GetWorldGrid();
}

bool USettingsLoader::LoadConfig()
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

	if (ConfigData.find(FString("GridSpacing")) != ConfigData.end())
	{
		try
		{
			float GridSpacing = std::stof(std::string(*(ConfigData[FString("GridSpacing")])));
			WorldGrid->SetSpacing(GridSpacing);
		}catch (...)
		{
			WorldGrid->SetSpacing(1.0f);
		}
	}

	return true;
}

bool USettingsLoader::SaveConfig()
{
	ConfigData[FString("CameraSensitivity")] = FString(std::to_string(Camera->GetCameraSensitivity()));
	ConfigData[FString("GridSpacing")] = FString(std::to_string(WorldGrid->GetSpacing()));

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