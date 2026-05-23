// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/CarreraGameMode.h"

#include "CarreraPlayerController.h"
#include "Public/CarreraGameState.h"
#include "Public/CarreraPlayerState.h"

ACarreraGameMode::ACarreraGameMode()
{
	GameStateClass=ACarreraGameState::StaticClass();
}

void ACarreraGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	ACarreraGameState* MiGameState = GetGameState<ACarreraGameState>();
	if (MiGameState)
	{
		MiGameState->SetEstadoCarrera(ECarreraEstado::Esperando);
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_InicioCarrera, this,&ACarreraGameMode::IniciarCarrera, 5.0f, false);
		
	}
}

void ACarreraGameMode::IniciarCarrera()
{
	ACarreraGameState* MiGameState = GetGameState<ACarreraGameState>();
	if (MiGameState)
	{
		MiGameState->SetEstadoCarrera(ECarreraEstado::Jugando);
	}
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

void ACarreraGameMode::FinalizarCarrera()
{
	ACarreraGameState* MiGameState = GetGameState<ACarreraGameState>();
	if (!MiGameState)return;
	
	MiGameState->SetEstadoCarrera(ECarreraEstado::Terminado);
	
	int32 PuntajeMaximo = -1;
	ACarreraPlayerState* JugadorGanador = nullptr;

	for (APlayerState*PS : MiGameState->PlayerArray)
	{
		ACarreraPlayerState*CarreraPS = Cast<ACarreraPlayerState>(PS);
		if (CarreraPS)
		{
			if (CarreraPS->PuntajeIndividual>PuntajeMaximo)
			{
				PuntajeMaximo=CarreraPS->PuntajeIndividual;
				JugadorGanador=CarreraPS;
			}
		}
	}
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ACarreraPlayerController*PC = Cast<ACarreraPlayerController>(It->Get());
		if (PC)
		{
			bool bGano = (PC->PlayerState==JugadorGanador);
			
			PC->Client_MostrarPantallaFin(bGano);
		}
	}
}
