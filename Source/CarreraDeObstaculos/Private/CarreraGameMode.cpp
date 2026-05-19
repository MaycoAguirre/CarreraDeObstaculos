// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/CarreraGameMode.h"
#include "Public/CarreraGameState.h"
#include "Public/CarreraPlayerState.h"

ACarreraGameMode::ACarreraGameMode()
{
	GameStateClass=ACarreraGameState::StaticClass();
}

void ACarreraGameMode::RegistrarLlegadaJugador(AController* JugadorController)
{
	ACarreraGameState* MiGameState = GetGameState<ACarreraGameState>();
	
	if (MiGameState && JugadorController)
	{
		MiGameState->JugadoresLlegados++;
		
		int32 PuntosOtorgados = 10;
		if (MiGameState->JugadoresLlegados == 1)PuntosOtorgados = 100;
		else if (MiGameState->JugadoresLlegados == 2)PuntosOtorgados = 50;
		else if (MiGameState->JugadoresLlegados== 3)PuntosOtorgados = 20;
		ACarreraPlayerState* PS = JugadorController->GetPlayerState<ACarreraPlayerState>();
		if (PS)
		{
			PS->SumarPuntos(PuntosOtorgados);
		}
	}
}
