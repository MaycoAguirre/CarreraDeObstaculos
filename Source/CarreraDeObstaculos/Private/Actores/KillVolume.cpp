// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actores/KillVolume.h"
#include "Components/BoxComponent.h"
#include "CarreraDeObstaculosCharacter.h"


// Sets default values
AKillVolume::AKillVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("TriggerBox"));
	RootComponent = TriggerBox;
	
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	TriggerBox-> OnComponentBeginOverlap.AddDynamic(this, &AKillVolume::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AKillVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKillVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()&&OtherActor)
	{
		ACarreraDeObstaculosCharacter* Jugador = Cast<ACarreraDeObstaculosCharacter>(OtherActor);
		if (Jugador)
		{
			Jugador->EjecutarRespawn();
		}
	}
}

// Called every frame
void AKillVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

