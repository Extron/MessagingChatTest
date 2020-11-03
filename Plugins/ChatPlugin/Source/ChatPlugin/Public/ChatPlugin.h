#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUICommandList;


class FChatPluginModule : public IModuleInterface
{
	#pragma region Fields
	
private:
	TSharedPtr<FUICommandList> pluginCommands;

	#pragma endregion


	#pragma region Lifecycle

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	#pragma endregion


	#pragma region Action Handlers
	
private:
	void OpenChatWindow1();
	void OpenChatWindow2();

	#pragma endregion
};
