// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "PuzzlePlatformerGameMode.h"
// #include "LobbyGameMode.generated.h"

/**
 * 
 */
// UCLASS()
class /*PUZZLEPLATFORMER_API*/ ALobbyGameMode 
{
	// GENERATED_BODY()

public:
	// void PostLogin(APlayerController* NewPlayer) override;
	// void Logout(AController* Exiting) override;

private:
	uint32 NumberOfPlayers = 0;
	uint32 RequiredPlayers = 2;
	FTimerHandle TimerHandle;
	
	void StartGame();
};
