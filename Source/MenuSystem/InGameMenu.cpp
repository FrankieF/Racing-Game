// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	if (!Super::Initialize()) return false;
	if (CancelButton == nullptr) return false;
		CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CloseInGameMenu);
	if (QuitButton == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("QUIT QUIT QUIT buttton not found."));
		return false;
	}
		QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitToMainMenu);
	
	return true;
}


void UInGameMenu::QuitToMainMenu()
{
	if (MenuInterface != nullptr)
	{
		TearDown();
		MenuInterface->LoadMainMenu();
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Cancel buttton not found."));
}

void UInGameMenu::CloseInGameMenu()
{
	TearDown();
}
