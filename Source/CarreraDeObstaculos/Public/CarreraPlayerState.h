// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CarreraPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CARRERADEOBSTACULOS_API ACarreraPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ACarreraPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(ReplicatedUsing = OnRep_PuntajeIndividual, BlueprintReadOnly, Category = "Estadisticas")
	int32 PuntajeIndividual;
	
	UFUNCTION()
	void OnRep_PuntajeIndividual();
	
	void SumarPuntos(int32 PuntosASumar);
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Carrera")
	int32 FinalPosition = 0;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Carrera")
	float FinishTime = 0.0f;
	
	UPROPERTY(ReplicatedUsing = OnRep_FinishedRace, BlueprintReadOnly, Category = "Carrera")
	bool bFinishedRace = false;
	
	UFUNCTION()
	void OnRep_FinishedRace();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void MostrarPantallaResultado();
	
};
