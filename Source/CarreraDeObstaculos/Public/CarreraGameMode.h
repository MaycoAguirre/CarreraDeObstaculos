// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CarreraGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CARRERADEOBSTACULOS_API ACarreraGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ACarreraGameMode();
	
	UFUNCTION(BlueprintCallable, Category = "Logica")
	void RegistrarLlegadaJugador(AController* JugadorController);
	
	
};
