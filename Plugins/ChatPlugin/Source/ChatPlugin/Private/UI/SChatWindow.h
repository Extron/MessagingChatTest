#pragma once

#include "CoreMinimal.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SCompoundWidget.h"
#include "MessageEndpoint.h"
#include "MessageEndpointBuilder.h"

#include "Messaging/ChatMessage.h"


class SEditableTextBox;


class SChatWindow : public SCompoundWidget
{
	#pragma region Fields

private:
	TArray<TSharedPtr<FChatMessage>> chatHistory;
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> chatEndpoint;
	
	TAttribute<FChatMessageAuthor> owningAuthorAttr;



	#pragma endregion


	#pragma region UI Controls

private:
	TSharedPtr<SEditableTextBox> messageTextBox;
	TSharedPtr< SListView< TSharedPtr<FChatMessage> > > chatHistoryListView;

	#pragma endregion


	#pragma region Constructors

public:
	SLATE_BEGIN_ARGS(SChatWindow) :
		_Author() {}
		SLATE_ATTRIBUTE(FChatMessageAuthor, Author)
	SLATE_END_ARGS()

	virtual ~SChatWindow();

	void Construct(const FArguments& args);

	#pragma endregion


	#pragma region Event Handlers

private:
	FReply OnSendButtonClicked();
	void OnMessageTextCommitted(const FText& newText, ETextCommit::Type commitInfo);
	TSharedRef<ITableRow> OnGenerateChatHistoryRow(TSharedPtr<FChatMessage> chatMessage, const TSharedRef<STableViewBase>& ownerTable);

	#pragma endregion


	#pragma region Messaging

private:
	void SendMessage(FString message);
	void OnChatMessageReceived(const FChatMessage& message, const IMessageContextRef& context);

	#pragma endregion


	#pragma region Window Management

private:
	static TMap< FString, TWeakPtr<SWindow> > OpenWindows;

public:
	static void ShowWindow(FString windowId, FText windowName, FText authorName);

	#pragma endregion
};