#include "Font.h"
#include <fstream>

UFont::UFont()
{
	Font = nullptr;
	Texture = nullptr;
}

UFont::~UFont()
{
}	

bool UFont::Create(ID3D11Device* Device, const wchar_t* FontFileName, const wchar_t* TextureFileName)
{
	Texture = new LegacyTexture();
	bool result;
	result = LoadFontData(FontFileName);
	if (!result)
	{
		return false;
	}
	result = LoadTexture(Device, TextureFileName);
	if (!result)
	{
		return false;
	}
	return true;
}

void UFont::Release()
{
	ReleaseFontData();
	ReleaseTexture();
}

bool UFont::LoadFontData(const wchar_t* FontFileName) {
	std::ifstream fin;
	int i;
	char temp;
	// 폰트 데이터를 저장할 배열을 생성합니다.
	Font = new FontType[95];
	if (!Font)
	{
		return false;
	}
	// 폰트 사이즈 데이터 파일을 열어서 읽어들임.
	fin.open(FontFileName);
	if (fin.fail())
	{
		return false;
	}
	// 95개의 아스키문자를 읽어들임.
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin >> Font[i].left;
		fin >> Font[i].right;
		fin >> Font[i].size;
	}
	// 파일을 닫음.
	fin.close();
	return true;
}

bool UFont::LoadTexture(ID3D11Device* Device, const wchar_t* TextureFileName)
{
	bool Result;
	Result = Texture->Create(Device, TextureFileName);
	if (FAILED(Result))
	{
		return false;
	}
	return true;
}

void UFont::ReleaseFontData()
{
	if (Font)
	{
		delete[] Font;
		Font = nullptr;
	}
}

void UFont::ReleaseTexture()
{
	if (Texture)
	{
		Texture->Release();
		delete Texture;
		Texture = nullptr;
	}
}	

void UFont::BuildVertexArray(void* Vertices, const FString Sentence, int ScreenWidth, int ScreenHeight) 
{
	VertexType* VertexPtr;
	float DrawX = 0;
	float DrawY = 0;
	float DrawZ = 0;
	int numLetters, index, i, letter;

	// 버텍스 배열에 대한 포인터를 설정합니다.
	VertexPtr = (VertexType*)Vertices;

	// 문자열의 길이를 저장합니다.
	numLetters = Sentence.Len();

	index = 0;

	for (i = 0; i < numLetters; i++) {
		letter = ((int)Sentence[i]) - 32;

		// 글자 간격은 최소 3.0f로 설정합니다.
		if (letter == 0) {
			DrawX = DrawX + 3.0f / (float)ScreenWidth;
		}
		else {
			float NDC_Size = Font[letter].size / (float)ScreenWidth;
			float NDC_Height = 32.0f / (float)ScreenHeight;

			VertexPtr[index].position = FVector(DrawZ, DrawX, DrawY);  // Top left.
			VertexPtr[index].texture = FVector(Font[letter].left, 0.0f, 0.0f);
			index++;
			VertexPtr[index].position = FVector(DrawZ, DrawX + NDC_Size, DrawY - NDC_Height);  // Bottom right.
			VertexPtr[index].texture = FVector(Font[letter].right, 1.0f, 0.0f);
			index++;
			VertexPtr[index].position = FVector(DrawZ, DrawX, DrawY - NDC_Height);  // Bottom left.
			VertexPtr[index].texture = FVector(Font[letter].left, 1.0f, 0.0f);
			index++;
			VertexPtr[index].position = FVector(DrawZ, DrawX, DrawY);  // Top left.
			VertexPtr[index].texture = FVector(Font[letter].left, 0.0f, 0.0f);
			index++;
			VertexPtr[index].position = FVector(DrawZ, DrawX + NDC_Size, DrawY);  // Top right.
			VertexPtr[index].texture = FVector(Font[letter].right, 0.0f, 0.0f);
			index++;
			VertexPtr[index].position = FVector(DrawZ, DrawX + NDC_Size, DrawY - NDC_Height);  // Bottom right.
			VertexPtr[index].texture = FVector(Font[letter].right, 1.0f, 0.0f);
			index++;
			DrawX = DrawX + NDC_Size + 1.0f / (float)ScreenWidth;
		}
	}
	return;
}

