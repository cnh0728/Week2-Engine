#pragma once
#include <string>
#include<unordered_map>
#include"Core/Container/String.h"

class UDeveloperSettings
{
public:
	UDeveloperSettings(const FString& InConfigFilename);

	virtual bool LoadConfig();
	virtual bool SaveConfig() = 0;

protected:
	FString ConfigFilename;
	std::unordered_map < FString, FString> ConfigData;
};