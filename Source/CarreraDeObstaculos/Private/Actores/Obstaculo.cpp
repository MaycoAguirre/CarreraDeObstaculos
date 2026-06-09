// Fill out your copyright notice in the Description page of Project Settings.


#include "Actores/Obstaculo.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"


// Sets default values
AObstaculo::AObstaculo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetReplicates(true);
	SetReplicateMovement(true);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	
	IntervaloCambioAleatorio = 3.0f;
	VelocidadGiroMaxima = 250.0f;
	VelocidadRotacionActual = FRotator::ZeroRotator;
	FuerzaImpactoHorizontal = 1500.0f;
	FuerzaImpactoVertical = 800.0f;
	
	MeshComp->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AObstaculo::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		
		CambiarMovimientoAleatorio();
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_CambioMovimiento, this, &AObstaculo::CambiarMovimientoAleatorio, IntervaloCambioAleatorio, true);
	}
}

void AObstaculo::CambiarMovimientoAleatorio()
{
	if (HasAuthority())
	{
		float GiroAleatorio = FMath::FRandRange(-VelocidadGiroMaxima, VelocidadGiroMaxima);

		if (FMath::Abs(GiroAleatorio)< 40.0f)
		{
			GiroAleatorio= (GiroAleatorio>= 0.0f)? 60.0f : -60.0f;
		}
		
		VelocidadRotacionActual = FRotator(0.0f, GiroAleatorio, 0.0f);
	}
}

// Called every frame
void AObstaculo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		AddActorLocalRotation(VelocidadRotacionActual * DeltaTime);
		
		TArray<AActor*> JugadoresTocados;
		
		MeshComp->GetOverlappingActors(JugadoresTocados, ACharacter::StaticClass());
		
		for (AActor* ActorTocado: JugadoresTocados)
		{
			ACharacter* Jugador = Cast<ACharacter>(ActorTocado);
			if (Jugador)
			{
				FVector DireccionGolpe = Jugador->GetActorLocation() - GetActorLocation();
				DireccionGolpe.Z = 0.0f;
				DireccionGolpe.Normalize();
				
				FVector VectorLanzamiento = (DireccionGolpe * FuerzaImpactoHorizontal) + FVector(0.0f, 0.0f, FuerzaImpactoVertical);
				
				Jugador->LaunchCharacter(VectorLanzamiento,true, true);
			}
		}
	}
}



