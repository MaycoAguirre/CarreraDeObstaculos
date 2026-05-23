// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/CarreraGameState.h"

#include "CarreraGameMode.h"
#include "CarreraPlayerController.h"
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
	DOREPLIFETIME(ACarreraGameState, EstadoActual);
}

bool ACarreraGameState::PuedeMoverse() const
{
	return EstadoActual==ECarreraEstado::Jugando;
}

void ACarreraGameState::SetEstadoCarrera(ECarreraEstado NuevoEstado)
{
	if (HasAuthority())
	{
		EstadoActual = NuevoEstado;
		
		OnRep_TiempoRestante();
	}
}

void ACarreraGameState::OnRep_EstadoActual()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue ,"El Estado de la partida ha cambiado");
}

void ACarreraGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Reloj, this, &ACarreraGameState::DisminuirTiempo, 1.0f, true);
		
	}
}

void ACarreraGameState::DisminuirTiempo()
{
	if (TiempoRestante > 0)
	{
		TiempoRestante--;
		
		OnRep_TiempoRestante();
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Reloj);
		GetWorld()->GetAuthGameMode<ACarreraGameMode>()->FinalizarCarrera();
	}
}

void ACarreraGameState::OnRep_TiempoRestante()
{
	ACarreraPlayerController* PControllerLocal = Cast<ACarreraPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PControllerLocal)
	{
		PControllerLocal->ActualizarTiempoHUD(TiempoRestante);
	}
}