#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Styling/ChatPluginStyle.h"

class FChatPluginCommands : public TCommands<FChatPluginCommands>
{

	#pragma region Properties

public:
	TSharedPtr<FUICommandInfo> OpenChatWindow1;
	TSharedPtr<FUICommandInfo> OpenChatWindow2;

	#pragma endregion


	#pragma region Constructors

public:
	FChatPluginCommands() :
		TCommands<FChatPluginCommands>(TEXT("ChatPlugin"), NSLOCTEXT("Contexts", "ChatPlugin", "ChatPlugin Plugin"), NAME_None, FChatPluginStyle::GetStyleSetName())
	{ }

	#pragma endregion


	#pragma region Lifecycle

	virtual void RegisterCommands() override;

	#pragma endregion
};
