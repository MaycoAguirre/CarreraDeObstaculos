// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CarreraGameState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECarreraEstado:uint8
{
	Esperando UMETA(DisplayName="Esperando Jugadores"),
	Jugando UMETA(DisplayName="Carrera en Progreso"),
	Terminado UMETA(DisplayName="Carrera Terminada"),
};

UCLASS()
class CARRERADEOBSTACULOS_API ACarreraGameState : public AGameState
{
	GENERATED_BODY()
	
public: 
	ACarreraGameState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(ReplicatedUsing="OnRep_EstadoActual", BlueprintReadOnly, Category="Estado")
	ECarreraEstado EstadoActual;
	
	UFUNCTION()
	void OnRep_EstadoActual();
	
	UFUNCTION(BlueprintCallable, Category="Estado")
	bool PuedeMoverse()const;
	
	void SetEstadoCarrera(ECarreraEstado NuevoEstado);
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Estado")
	int32 JugadoresLlegados;
	
	UPROPERTY(ReplicatedUsing= OnRep_TiempoRestante, BlueprintReadOnly, Category="Estado")
	int32 TiempoRestante;
	
	UFUNCTION()
	void OnRep_TiempoRestante();
	
	FTimerHandle TimerHandle_Reloj;
	void DisminuirTiempo();

protected: 
	virtual void BeginPlay() override;
	
	
};
