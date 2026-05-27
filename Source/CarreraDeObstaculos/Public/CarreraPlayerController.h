// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CarreraPlayerController.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class CARRERADEOBSTACULOS_API ACarreraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ClaseHUD;
	
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* HUDInstanciado;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ActualizarPuntajeHUD(int32 NuevoPuntaje);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void MostrarEfectoRespawnHUD();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ActualizarTiempoHUD(int32 NuevoTiempo);
	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = "UI")
	void ActualizarEstadoHUD(ECarreraEstado NuevoEstado);
	
	UFUNCTION(Client, Reliable)
	void Client_MostrarPantallaFin(bool bEsGanador);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void MostrarFinDeJuegoHUD(bool bEsGanador);
};
