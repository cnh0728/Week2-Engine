#include "UDeveloperSettings.h"
#include <fstream>

UDeveloperSettings::UDeveloperSettings(const FString& InConfigFilename)
	:ConfigFilename(InConfigFilename)
{
}

bool UDeveloperSettings::LoadConfig()
{
	ConfigData.clear();

	std::ifstream InFile(*ConfigFilename);
	if (!InFile.is_open())
		return false;

	std::string line;
	while (std::getline(InFile, line))
	{
		// 주석 제거 및 좌우 공백 제거
		size_t commentPos = line.find(';');
		if (commentPos != std::string::npos)
			line = line.substr(0, commentPos);

		// std::string을 FString으로 변환
		FString LineFString(line);

		// 좌우 공백 제거
		LineFString = LineFString.Trim();
		if (LineFString.IsEmpty())
			continue;

		// "Key=Value" 형식으로 파싱
		int32 EqualPos = LineFString.Find(FString("="));
		if (EqualPos == INDEX_NONE)
			continue;

		FString Key = LineFString.Left(EqualPos).Trim();
		FString Value = LineFString.Right(LineFString.Len() - EqualPos - 1).Trim();
		ConfigData[Key] = Value;
	}
	InFile.close();
	return true;
}