// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstaculo.generated.h"

UCLASS()
class CARRERADEOBSTACULOS_API AObstaculo : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObstaculo();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impacto")
	float FuerzaImpactoHorizontal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impacto")
	float FuerzaImpactoVertical;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Componentes")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuraciones")
	float IntervaloCambioAleatorio;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuraciones")
	float VelocidadGiroMaxima;
	
private:
	
	FRotator VelocidadRotacionActual;
	
	FTimerHandle TimerHandle_CambioMovimiento;
	
	void CambiarMovimientoAleatorio();
};
