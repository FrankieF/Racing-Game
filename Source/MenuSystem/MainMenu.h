// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	uint16 NumberOfPlayers;
	uint16 MaxPlayers;
	FString Name;
	FString HostUserName;
};

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMER_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer &ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 index);
	
protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostServerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;
	
    UPROPERTY(meta = (BindWidget))
    class UButton* CancelHostMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;
	
	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* HostName;

	TSubclassOf<class UUserWidget> ServerRowClass;

	UFUNCTION()
	void HostServer();
	
	UFUNCTION()
	void OpenJoinMenu();
	
	UFUNCTION()
	void OpenMainMenu();
	UFUNCTION()
	void OpenHostMenu();
	
	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void Quit();

	void UpdateChildren();

	TOptional<uint32> SelectedIndex;
};
