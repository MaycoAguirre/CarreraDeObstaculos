// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CarreraGameState.generated.h"

/**
 * 
 */
UCLASS()
class CARRERADEOBSTACULOS_API ACarreraGameState : public AGameState
{
	GENERATED_BODY()
	
public: 
	ACarreraGameState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Estado")
	int32 TiempoRestante;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Estado")
	int32 JugadoresLlegados;
	
};
