#include "ChatPlugin.h"

#include "Styling/ChatPluginStyle.h"
#include "Commands/ChatPluginCommands.h"
#include "UI/SChatWindow.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"


#define LOCTEXT_NAMESPACE "FChatPluginModule"


#pragma region Lifecycle

void FChatPluginModule::StartupModule()
{
	FChatPluginStyle::Initialize();
	FChatPluginStyle::ReloadTextures();

	FChatPluginCommands::Register();
	
	pluginCommands = MakeShareable(new FUICommandList);

	pluginCommands->MapAction(FChatPluginCommands::Get().OpenChatWindow1, FExecuteAction::CreateRaw(this, &FChatPluginModule::OpenChatWindow1), FCanExecuteAction());
	pluginCommands->MapAction(FChatPluginCommands::Get().OpenChatWindow2, FExecuteAction::CreateRaw(this, &FChatPluginModule::OpenChatWindow2), FCanExecuteAction());

	FLevelEditorModule& levelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	TSharedPtr<FExtender> toolbarExtender = MakeShareable(new FExtender);
	toolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, pluginCommands, FToolBarExtensionDelegate::CreateLambda([&](FToolBarBuilder& builder)
	{
		builder.AddComboButton(
			FUIAction(),
			FOnGetContent::CreateLambda([&]()
			{
				FMenuBuilder menuBuilder(true, pluginCommands);

				menuBuilder.AddMenuEntry(FChatPluginCommands::Get().OpenChatWindow1);
				menuBuilder.AddMenuEntry(FChatPluginCommands::Get().OpenChatWindow2);

				return menuBuilder.MakeWidget();
			}),
			LOCTEXT("ChatCombo_Label", "Chat"),
			LOCTEXT("ChatCombo_Tooltip", "Chat Windows"),
			FSlateIcon(FChatPluginStyle::GetStyleSetName(), "ChatPlugin.Icon")
		);
	}));
		
	levelEditorModule.GetToolBarExtensibilityManager()->AddExtender(toolbarExtender);
}

void FChatPluginModule::ShutdownModule()
{
	FChatPluginStyle::Shutdown();

	FChatPluginCommands::Unregister();
}

#pragma endregion


#pragma region Action Handlers

void FChatPluginModule::OpenChatWindow1()
{
	SChatWindow::ShowWindow("WindowOne", LOCTEXT("ChatWindowOne_Name", "Chat One"), LOCTEXT("ChatWindowOne_Author", "Author One"));
}

void FChatPluginModule::OpenChatWindow2()
{
	SChatWindow::ShowWindow("WindowTwo", LOCTEXT("ChatWindowTwo_Name", "Chat Two"), LOCTEXT("ChatWindowTwo_Author", "Author Two"));
}

#pragma endregion


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FChatPluginModule, ChatPlugin)