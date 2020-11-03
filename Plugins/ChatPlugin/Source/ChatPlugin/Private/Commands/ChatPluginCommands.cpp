#include "ChatPluginCommands.h"

#define LOCTEXT_NAMESPACE "FChatPluginModule"


#pragma region Lifecycle

void FChatPluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenChatWindow1, "Chat Window 1", "Open chat window 1", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(OpenChatWindow2, "Chat Window 2", "Open chat window 2", EUserInterfaceActionType::Button, FInputGesture());
}

#pragma endregion


#undef LOCTEXT_NAMESPACE
