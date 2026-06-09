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
		if (!JugadoresQueLlegaron.Contains(JugadorController))
		{
			JugadoresQueLlegaron.Add(JugadorController);
			MiGameState->JugadoresLlegados = JugadoresQueLlegaron.Num();//Actualizamos la Cantidad
		
			//Definimos el mensaje segun el puesto
			FString MensajePuesto;
			if (JugadoresQueLlegaron.Num()==1)MensajePuesto=TEXT("¡Has Ganado - Primer Puesto!");
			else if (JugadoresQueLlegaron.Num()==2)MensajePuesto = TEXT("Segundo Puesto");
			else if (JugadoresQueLlegaron.Num()==3)MensajePuesto = TEXT("Tercer Puesto");
			else MensajePuesto = TEXT("Has Finalizado");
			
			//Le mandamos el mensaje solo a este jugador
			ACarreraPlayerController* PC = Cast<ACarreraPlayerController>(JugadorController);
			if (PC)
			{
				PC->Client_MostrarResultado(MensajePuesto);
			}
			if (JugadoresQueLlegaron.Num()>= GetNumPlayers())
			{
				FinalizarCarrera();
			}
		}
	}
}

void ACarreraGameMode::FinalizarCarrera()
{
	ACarreraGameState* MiGameState = GetGameState<ACarreraGameState>();
	if (MiGameState)
	{
		MiGameState->SetEstadoCarrera(ECarreraEstado::Terminado);
	}
	//Recorremos a todos los jugadores de la partida
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ACarreraPlayerController* PC = Cast<ACarreraPlayerController>(It->Get());
		
		//Si el jugador no esta en la lista de los jugadores que llegaron, significa que se le acabo el tiempo
		if (PC && !JugadoresQueLlegaron.Contains(PC))
		{
			PC->Client_MostrarResultado(TEXT("Tiempo Terminado"));
		}
	}
}
