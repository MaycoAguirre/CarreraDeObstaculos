// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/CarreraGameState.h"
#include "CarreraGameMode.h"
#include "CarreraPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerState.h"

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
		
		if (EstadoActual == ECarreraEstado::Jugando)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_Reloj, this, &ACarreraGameState::DisminuirTiempo, 1.0f, true);
		
		}else if (EstadoActual==ECarreraEstado::Terminado)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Reloj);
		}
		
		OnRep_TiempoRestante();
	}
}

void ACarreraGameState::OnRep_EstadoActual()
{
	ACarreraPlayerController* PCLocal = Cast<ACarreraPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PCLocal)
	{
		PCLocal->ActualizarEstadoHUD(EstadoActual);
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue ,"El Estado de la partida ha cambiado");
}

void ACarreraGameState::BeginPlay()
{
	Super::BeginPlay();
	
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

TArray<APlayerState*> ACarreraGameState::GetJugadoresOrdenadosPorDistancia()
{
	TArray<APlayerState*> ListaOrdenada = PlayerArray;
	
	TArray<AActor*> ActoresMeta;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Meta"),ActoresMeta);
	
	if (ActoresMeta.Num() == 0|| ListaOrdenada.Num()<= 1)
	{
		return ListaOrdenada;
	}
	
	FVector UbicacionMeta = ActoresMeta[0]->GetActorLocation();
	
	for (int32 i = 0; i < ListaOrdenada.Num() - 1; i++)
	{
		for (int32 j = 0; j < ListaOrdenada.Num() - i - 1; j++)
		{
			APawn* PawnA = ListaOrdenada[j]-> GetPawn();
			APawn* PawnB = ListaOrdenada[j+1]->GetPawn();
			
			float DistA = PawnA ? FVector::Dist(PawnA->GetActorLocation(), UbicacionMeta) : 999999.0f;
			float DistB = PawnB ? FVector::Dist(PawnB->GetActorLocation(), UbicacionMeta) : 999999.0f;
			
			if (DistA > DistB)
			{
				APlayerState* Temp = ListaOrdenada[j];
				ListaOrdenada[j] = ListaOrdenada[j+1];
				ListaOrdenada[j+1] = Temp;
			}
		}
	}
	
	return ListaOrdenada;
}

void ACarreraGameState::OnRep_TiempoRestante()
{
	ACarreraPlayerController* PControllerLocal = Cast<ACarreraPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PControllerLocal)
	{
		PControllerLocal->ActualizarTiempoHUD(TiempoRestante);
	}
}
