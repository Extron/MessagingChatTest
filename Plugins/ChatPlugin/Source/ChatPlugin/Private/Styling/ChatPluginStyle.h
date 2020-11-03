#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FSlateStyleSet;


class FChatPluginStyle
{
	#pragma region Lifecycle

public:
	static void Initialize();
	static void Shutdown();
	static void ReloadTextures();

	static FName GetStyleSetName();

private:
	static TSharedRef<FSlateStyleSet> Create();

	#pragma endregion


	#pragma region Singleton

private:
	static TSharedPtr<FSlateStyleSet> StyleInstance;

public:
	static const ISlateStyle& Get();

	#pragma endregion
};