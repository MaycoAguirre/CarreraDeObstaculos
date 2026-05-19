// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/CarreraGameState.h"
#include "Net/UnrealNetwork.h"

ACarreraGameState::ACarreraGameState()
{
	TiempoRestante = 120;
	JugadoresLlegados = 0;
}

void ACarreraGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACarreraGameState, TiempoRestante);
	DOREPLIFETIME(ACarreraGameState, JugadoresLlegados);
}
