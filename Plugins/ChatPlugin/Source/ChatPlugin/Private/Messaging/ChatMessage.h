#pragma once

#include "CoreMinimal.h"

#include "ChatMessage.generated.h"


USTRUCT()
struct FChatMessageAuthor
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGuid AuthorId;

	UPROPERTY()
	FText AuthorDisplayName;

	FChatMessageAuthor() :
		AuthorId(FGuid::NewGuid()),
		AuthorDisplayName()
	{ }

	FChatMessageAuthor(FText displayName) :
		AuthorId(FGuid::NewGuid()),
		AuthorDisplayName(displayName)
	{ }
};

USTRUCT()
struct FChatMessage
{
	GENERATED_BODY()


public:
	UPROPERTY()
	FChatMessageAuthor Author;

	UPROPERTY()
	FString Message;

	UPROPERTY()
	FGuid MessageId;

	FChatMessage() :
		Author(),
		Message(),
		MessageId(FGuid::NewGuid())
	{ }
};