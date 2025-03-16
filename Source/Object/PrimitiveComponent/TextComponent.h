#pragma once
#include "Object/USceneComponent.h"

class UTextComponent : public USceneComponent {
	using Super = USceneComponent;
public:
	UTextComponent();
	virtual ~UTextComponent() = default;
	void SetText(const char* text);
	const char* GetText() const;
};