// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "Components/Button.h"

#include "MainMenu.h"

void UServerRow::SetUp(UMainMenu* InParent, uint32 InIndex)
{
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
	Parent = InParent;
	Index = InIndex;
}

void UServerRow::OnClicked()
{
	if (Parent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Server Row is null."));
		return;
	}
	Parent->SelectIndex(Index);
}