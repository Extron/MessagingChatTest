#include "ChatPluginStyle.h"
#include "ChatPlugin.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"


#pragma region Lifecycle

void FChatPluginStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FChatPluginStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

void FChatPluginStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

FName FChatPluginStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ChatPluginStyle"));
	return StyleSetName;
}

const FVector2D Icon40x40(40.0f, 40.0f);

TSharedRef< FSlateStyleSet > FChatPluginStyle::Create()
{
	TSharedRef< FSlateStyleSet > style = MakeShareable(new FSlateStyleSet("ChatPluginStyle"));
	style->SetContentRoot(IPluginManager::Get().FindPlugin("ChatPlugin")->GetBaseDir() / TEXT("Resources"));

	style->Set("ChatPlugin.Icon", new FSlateImageBrush(style->RootToContentDir(TEXT("Icon40"), TEXT(".png")), Icon40x40));

	style->Set("ChatWindow.ChatHistory.BackgroundBrush", new FSlateColorBrush(FLinearColor(FColor(62, 62, 62))));

	style->Set("ChatWindow.ChatHistory.SelfMessageColor", FLinearColor::White);
	style->Set("ChatWindow.ChatHistory.OtherMessageColor", FLinearColor::Yellow);

	return style;
}

#pragma endregion


#pragma region Singleton

TSharedPtr< FSlateStyleSet > FChatPluginStyle::StyleInstance = nullptr;

const ISlateStyle& FChatPluginStyle::Get()
{
	return *StyleInstance;
}

#pragma endregion
