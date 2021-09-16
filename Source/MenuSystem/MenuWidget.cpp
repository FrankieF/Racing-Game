// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* Menu)
{
	this->MenuInterface = Menu;
}

void UMenuWidget::SetUp()
{
	this->AddToViewport();
	this->bIsFocusable = true;
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null."));
		return;
	}
	FInputModeUIOnly InputData;
	InputData.SetWidgetToFocus(this->TakeWidget());
	InputData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputData);
	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::TearDown()
{
	this->RemoveFromViewport();
	this->bIsFocusable = false;
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null."));
		return;
	}
	FInputModeGameOnly InputData;
	PlayerController->SetInputMode(InputData);
	PlayerController->bShowMouseCursor = false;
}