#include "SChatWindow.h"

#include "Styling/ChatPluginStyle.h"

#include "Brushes/SlateColorBrush.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Views/SListView.h"
#include "EditorStyleSet.h"


#define LOCTEXT_NAMESPACE "SChatWindow"


#pragma region Constructors

SChatWindow::~SChatWindow()
{
	if (chatEndpoint.IsValid())
	{
		chatEndpoint->Disable();
		chatEndpoint.Reset();
	}
}

void SChatWindow::Construct(const FArguments& args)
{
	owningAuthorAttr = args._Author;

	chatEndpoint = FMessageEndpoint::Builder("SChatWindow")
		.Handling<FChatMessage>(this, &SChatWindow::OnChatMessageReceived)
		.Build();

	if (chatEndpoint.IsValid())
		chatEndpoint->Subscribe<FChatMessage>();

	ChildSlot
	[
		SNew(SVerticalBox)

		+SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(4, 4, 4, 4)
		[
			SNew(SBorder)
			.BorderImage(FChatPluginStyle::Get().GetBrush("ChatWindow.ChatHistory.BackgroundBrush"))
			[
				SNew(SScrollBox)

				+SScrollBox::Slot()
				[
					SAssignNew(chatHistoryListView, SListView< TSharedPtr<FChatMessage> >)
					.ItemHeight(16.0f)
					.ListItemsSource(&chatHistory)
					.OnGenerateRow(this, &SChatWindow::OnGenerateChatHistoryRow)
					.SelectionMode(ESelectionMode::None)
				]
			]
		]

		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(4, 4, 4, 4)
		[
			SNew(SHorizontalBox)

			+SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.Padding(4, 4, 4, 4)
			[
				SAssignNew(messageTextBox, SEditableTextBox)
				.OnTextCommitted(this, &SChatWindow::OnMessageTextCommitted)
			]

			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(4, 4, 4, 4)
			[
				SNew(SButton)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.OnClicked(this, &SChatWindow::OnSendButtonClicked)
				.Text(LOCTEXT("SendButton_Text", "Send"))
				.ToolTipText(LOCTEXT("SendButton_Tooltip", "Sends the message"))
			]
		]
	];	
}

#pragma endregion


#pragma region Event Handlers

FReply SChatWindow::OnSendButtonClicked()
{
	SendMessage(messageTextBox->GetText().ToString());
	messageTextBox->SetText(FText::GetEmpty());

	return FReply::Handled();
}

void SChatWindow::OnMessageTextCommitted(const FText& newText, ETextCommit::Type commitInfo)
{
	if (commitInfo == ETextCommit::OnEnter)
	{
		SendMessage(newText.ToString());
		messageTextBox->SetText(FText::GetEmpty());
	}
}

TSharedRef<ITableRow> SChatWindow::OnGenerateChatHistoryRow(TSharedPtr<FChatMessage> chatMessage, const TSharedRef<STableViewBase>& ownerTable)
{
	bool isSelfMessage = chatMessage->Author.AuthorId == owningAuthorAttr.Get().AuthorId;
	FText author = isSelfMessage ? LOCTEXT("ChatWindow_Me", "Me") : chatMessage->Author.AuthorDisplayName;
	FText messageText = FText::Format(LOCTEXT("ChatWindow_MessageTemplate", "{0}: {1}"), author, FText::FromString(chatMessage->Message));
	FLinearColor messageColor = isSelfMessage ? FChatPluginStyle::Get().GetColor("ChatWindow.ChatHistory.SelfMessageColor") : FChatPluginStyle::Get().GetColor("ChatWindow.ChatHistory.OtherMessageColor");

	return SNew(STableRow<TSharedPtr<FChatMessage>>, ownerTable)
	[
		SNew(SBorder)
		.Padding(FMargin(4))
		.BorderImage(FEditorStyle::GetBrush("NoBorder"))
		[
			SNew(STextBlock)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
			.ColorAndOpacity(messageColor)
			.Text(messageText)
		]
	];
}

#pragma endregion


#pragma region Messaging

void SChatWindow::SendMessage(FString message)
{
	if (chatEndpoint.IsValid())
	{
		TSharedPtr<FChatMessage> chatMessage = MakeShareable(new FChatMessage());

		chatMessage->Author = owningAuthorAttr.Get();
		chatMessage->Message = message;

		chatHistory.Add(chatMessage);

		// Create a new copy of the chat message to send through the message buffer.
		chatEndpoint->Publish<FChatMessage>(new FChatMessage(*chatMessage.Get()));

		chatHistoryListView->RequestListRefresh();
	}
}

void SChatWindow::OnChatMessageReceived(const FChatMessage& message, const IMessageContextRef& context)
{
	if (message.Author.AuthorId != owningAuthorAttr.Get().AuthorId)
	{
		TSharedPtr<FChatMessage> chatMessage = MakeShareable(new FChatMessage(message));
		chatHistory.Add(chatMessage);
		chatHistoryListView->RequestListRefresh();
	}
}

#pragma endregion


#pragma region Window Management

TMap< FString, TWeakPtr<SWindow> > SChatWindow::OpenWindows = TMap< FString, TWeakPtr<SWindow> >();

void SChatWindow::ShowWindow(FString windowId, FText windowName, FText authorName)
{
	TSharedPtr<SWindow> existingWindow = nullptr;

	if (OpenWindows.Contains(windowId) && OpenWindows[windowId].IsValid())
	{
		existingWindow = OpenWindows[windowId].Pin();
		existingWindow->BringToFront();
		return;
	}
	else
	{
		existingWindow = SNew(SWindow)
			.Title(FText::Format(LOCTEXT("ChatWindow_Title", "Chat Window: {0}"), windowName))
			.SizingRule(ESizingRule::UserSized)
			.ClientSize(FVector2D(1000.f, 700.f))
			.AutoCenter(EAutoCenter::PreferredWorkArea)
			.SupportsMinimize(false);

		TSharedPtr<SWindow> RootWindow = FGlobalTabmanager::Get()->GetRootWindow();
		if (RootWindow.IsValid())
			FSlateApplication::Get().AddWindowAsNativeChild(existingWindow.ToSharedRef(), RootWindow.ToSharedRef());
		else
			FSlateApplication::Get().AddWindow(existingWindow.ToSharedRef());

		if (OpenWindows.Contains(windowId))
			OpenWindows[windowId] = existingWindow;
		else
			OpenWindows.Add(windowId, existingWindow);
	}

	existingWindow->SetContent(
		SNew(SChatWindow)
		.Author(FChatMessageAuthor(authorName))
	);
}

#pragma endregion


#undef LOCTEXT_NAMESPACE