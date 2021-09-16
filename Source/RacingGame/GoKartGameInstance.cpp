// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKartGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My Session Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UGoKartGameInstance::UGoKartGameInstance(const FObjectInitializer &ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (MenuWidgetBPClass.Class != nullptr)
	{
		MenuClass = MenuWidgetBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWidgetBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (InGameMenuWidgetBPClass.Class != nullptr)
	{
		InGameMenuClass = InGameMenuWidgetBPClass.Class;
	}
}

void UGoKartGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Online Subsytem is null."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Online Subsystem %s."), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UGoKartGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UGoKartGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UGoKartGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UGoKartGameInstance::OnJoinSessionComplete);
		}
	}

	if (GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UGoKartGameInstance::OnNetworkFailure);
	}
}

void UGoKartGameInstance::LoadMenuWidget()
{
	if (MenuClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Menu class is null."));
		return;
	}
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (Menu == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Menu is null"));
		return;
	}
	Menu->SetUp();
	Menu->SetMenuInterface(this);
}

void UGoKartGameInstance::LoadInGameMenu()
{
	if (InGameMenuClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("InGameMenu class is null."));
		return;
	}
	UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (InGameMenu == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("InGameMenu is null"));
		return;
	}
	InGameMenu->SetUp();
		InGameMenu->SetMenuInterface(this);
}

void UGoKartGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UGoKartGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot create session."));
		return;
	}
	if (Menu != nullptr)
		Menu->TearDown();
	
	UEngine* Engine = GetEngine();
	if (Engine == nullptr)
	    return;
    Engine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Begin Hosting"));

	UWorld* World = GetWorld();
	if (World == nullptr)
		return;
	World->ServerTravel("/Game/PuzzlePlatform/Maps/Lobby?listen");
}

void UGoKartGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UGoKartGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionSearch->MaxSearchResults = 100;
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session."));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UGoKartGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished Finding Session"));
		TArray<FServerData> ServerNames;
		for(const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session %s"), *SearchResult.GetSessionIdStr());
			FServerData Data;
			Data.Name = SearchResult.GetSessionIdStr();
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.NumberOfPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUserName = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
				Data.Name = "Could not find name.";
			ServerNames.Add(Data);
		}
		Menu->SetServerList(ServerNames);
	}
}

void UGoKartGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}

void UGoKartGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UGoKartGameInstance::Join(const uint32 Index)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;
	if (Menu != nullptr)
		Menu->TearDown();

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}


void UGoKartGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;
	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}
	UEngine* Engine = GetEngine();
	if (Engine == nullptr)
	    return;
	Engine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, FString::Printf(TEXT("Joining %s"), *Address));
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr)
		return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UGoKartGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UGoKartGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr)
		return;
	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}