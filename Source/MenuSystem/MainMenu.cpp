// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer &ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (ServerRowBPClass.Class != nullptr)
	{
		ServerRowClass = ServerRowBPClass.Class;
	}
}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize()) return false;
	if (HostButton == nullptr) return false;
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
	if (JoinButton == nullptr) return false;
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	if (JoinMenuButton == nullptr) return false;
		JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	if (BackButton == nullptr) return false;
		BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	if (ExitButton == nullptr) return false;
		ExitButton->OnClicked.AddDynamic(this, &UMainMenu::Quit);
	if (HostServerButton == nullptr) return false;
		HostServerButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	if (CancelHostMenuButton == nullptr) return false;
		CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	if (MenuSwitcher == nullptr) return false;
	
	return true;
}

void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		FString ServerName = HostName->Text.ToString();
		MenuInterface->Host(ServerName);
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher != nullptr && JoinMenu != nullptr)
		MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface != nullptr)
		MenuInterface->RefreshServerList();
}

void UMainMenu::OpenMainMenu()
{
	if (MenuSwitcher != nullptr && MainMenu != nullptr)
		MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::OpenHostMenu()
{
	if (MenuSwitcher != nullptr && HostMenu != nullptr)
		MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d."), SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set."));
	}
	if (MenuInterface == nullptr)
		return;
	if (MenuInterface != nullptr)
	{
		MenuInterface->Join(SelectedIndex.GetValue());
	}
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	ServerList->ClearChildren();
	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (Row == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Row is null"));
			return;
		}
		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostName->SetText(FText::FromString(ServerData.HostUserName));
		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.NumberOfPlayers, ServerData.MaxPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionText));
		Row->SetUp(this, i);
		++i;
		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(uint32 index)
{
	SelectedIndex = index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	if (ServerList == nullptr) return;
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = SelectedIndex.IsSet() && SelectedIndex.GetValue() == i;
		}
	}
}


void UMainMenu::Quit()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null."));
		return;
	}
	PlayerController->ConsoleCommand("quit");
}