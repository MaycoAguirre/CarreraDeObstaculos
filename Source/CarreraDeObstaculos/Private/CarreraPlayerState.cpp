// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/CarreraPlayerState.h"
#include "CarreraPlayerController.h"
#include "Net/UnrealNetwork.h"

ACarreraPlayerState::ACarreraPlayerState()
{
	PuntajeIndividual = 0;
	
}

void ACarreraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACarreraPlayerState, PuntajeIndividual);
	
}

void ACarreraPlayerState::SumarPuntos(int32 PuntosASumar)
{
	if (HasAuthority())
	{
		PuntajeIndividual += PuntosASumar;
		
		OnRep_PuntajeIndividual();
	}
}

void ACarreraPlayerState::OnRep_PuntajeIndividual()
{
	ACarreraPlayerController* PController = Cast<ACarreraPlayerController>(GetPlayerController());
	if (PController)
	{
		PController->ActualizarPuntajeHUD(PuntajeIndividual);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green ,"Puntaje Actualizado. Nuevo Total: %d"+ PuntajeIndividual);
	}
	
}
