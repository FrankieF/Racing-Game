// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "TimerManager.h"
// #include "PuzzlePlatformGameInstance.h"

// void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
// {
// 	// Super::PostLogin(NewPlayer);
// 	// ++NumberOfPlayers;
// 	//
// 	// if (NumberOfPlayers >= RequiredPlayers)
// 	// {
// 	// 	GetWorldTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::StartGame, 5);
// 	// }
// }
//
// void ALobbyGameMode::Logout(AController* Exiting)
// {
// 	// Super::Logout(Exiting);
// 	--NumberOfPlayers;
// }

void ALobbyGameMode::StartGame()
{
	// auto GameInstance = Cast<UPuzzlePlatformGameInstance>(GetGameInstance());
	// if (GameInstance != nullptr)
	// 	return;
	// GameInstance->StartSession();
	// UWorld* World = GetWorld();
	// if (World == nullptr)
	// 	return;
	// bUseSeamlessTravel = true;
	// World->ServerTravel("/Game/PuzzlePlatform/Maps/Game?listen");
}
