#include "UObject.h"
#include "Object/Class/Class.h"

UObject::UObject()
{

}

UObject::~UObject()
{
}

// UObject를 담는 UClass는 nullptr을 parent로 가짐
UClass* UObject::StaticClass()
{
	static UClass ClassPrivate(nullptr, "UObject");
	return &ClassPrivate;
}


inline bool UObject::IsA(UClass* TargetClass) const
{
	UClass* CurrentClass = GetClass();
	while (CurrentClass) {
		if (CurrentClass == TargetClass)
			return true;
		CurrentClass = CurrentClass->SuperClass;
	}
	return false;
}
