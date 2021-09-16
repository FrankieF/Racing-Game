// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GoKartGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RACINGGAME_API UGoKartGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
public:
	UGoKartGameInstance(const FObjectInitializer &ObjectInitializer);
	virtual void Init();
	
	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();
	
	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();
	
	UFUNCTION(Exec)
	void Host(FString ServerName) override;
	
	UFUNCTION(Exec)
	void Join(const uint32 Index) override;
	
	void LoadMainMenu() override;
	
	void RefreshServerList() override;
	
	void StartSession();
	
	private:
	TSubclassOf<class UUserWidget> MenuClass;
	class UMainMenu* Menu;
	
	TSubclassOf<class UUserWidget> InGameMenuClass;
	
	IOnlineSessionPtr SessionInterface;
	
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	
	FString DesiredServerName;
	void CreateSession();
};