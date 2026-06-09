// Fill out your copyright notice in the Description page of Project Settings.


#include "CarreraPlayerController.h"
#include "Blueprint/UserWidget.h"

void ACarreraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController()&&ClaseHUD)
	{
		HUDInstanciado = CreateWidget<UUserWidget>(this, ClaseHUD);

		if (HUDInstanciado)
		{
			HUDInstanciado->AddToViewport();
		}
	}
}

void ACarreraPlayerController::Client_MostrarResultado_Implementation(const FString& MensajeResultado)
{
	
	SetIgnoreMoveInput(true);
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
	
	MostrarFinDeJuegoHUD(MensajeResultado);
}
