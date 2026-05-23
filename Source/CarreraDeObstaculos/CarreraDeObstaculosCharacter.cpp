// Copyright Epic Games, Inc. All Rights Reserved.

#include "CarreraDeObstaculosCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CarreraDeObstaculos.h"
#include "CarreraGameState.h"
#include "Net/UnrealNetwork.h"

ACarreraDeObstaculosCharacter::ACarreraDeObstaculosCharacter()
{
	
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ACarreraDeObstaculosCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACarreraDeObstaculosCharacter, bEstaAgarrado);
}

void ACarreraDeObstaculosCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		UltimoCheckpoint = GetActorLocation();
	}
	
	VelocidadOriginal = GetCharacterMovement()->MaxWalkSpeed;
	bEstaAgarrado = false;
}

void ACarreraDeObstaculosCharacter::Jump()
{
	ACarreraGameState* MiGameState = GetWorld()->GetGameState<ACarreraGameState>();
	if (MiGameState&&!MiGameState->PuedeMoverse())return;
	if (bEstaAgarrado)return;
	Super::Jump();
}

void ACarreraDeObstaculosCharacter::IntentarAgarrar()
{
	ACarreraGameState* MiGameState = GetWorld()->GetGameState<ACarreraGameState>();
	if (MiGameState&&!MiGameState->PuedeMoverse())return;
	if (bPuedeAgarrar && !bEstaAgarrado)
	{
		Server_Agarrar();
	}
}

void ACarreraDeObstaculosCharacter::Server_Agarrar_Implementation()
{
	if (!bPuedeAgarrar)return;
	
	bPuedeAgarrar = false;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CooldownAgarre, this, &ACarreraDeObstaculosCharacter::ResetearCooldownAgarre, CooldownAgarre, false);
	
	FVector StartLocation= GetActorLocation();
	FVector EndLocation= StartLocation + (GetActorForwardVector()*100.0f);
	FHitResult HitResult;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(40.0f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult,StartLocation,EndLocation,FQuat::Identity,ECC_Pawn,Sphere, QueryParams);
	
	if (bHit)
	{
		ACarreraDeObstaculosCharacter* OtroJugador = Cast<ACarreraDeObstaculosCharacter>(HitResult.GetActor());
		if (OtroJugador)
		{
			OtroJugador->AplicarEfectoAgarre(DuracionAgarre);
		}
	}
	Multicast_EfectoAgarre();
}

void ACarreraDeObstaculosCharacter::Multicast_EfectoAgarre_Implementation()
{
	
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red ,"Efecto De Agarre a otro Jugador");
}

void ACarreraDeObstaculosCharacter::ResetearCooldownAgarre()
{
	bPuedeAgarrar=true;
}

void ACarreraDeObstaculosCharacter::AplicarEfectoAgarre(float Duracion)
{
	if (HasAuthority())
	{
		bEstaAgarrado=true;
		
		GetCharacterMovement()->MaxWalkSpeed=VelocidadOriginal*0.2f;
		OnRep_IsGrabbed();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Liberacion,this, &ACarreraDeObstaculosCharacter::FinalizarAgarre,DuracionAgarre, false);
		
	}
}

void ACarreraDeObstaculosCharacter::FinalizarAgarre()
{
	if (HasAuthority())
	{
		bEstaAgarrado=false;
		GetCharacterMovement()->MaxWalkSpeed=VelocidadOriginal;
		OnRep_IsGrabbed();
	}
}

void ACarreraDeObstaculosCharacter::OnRep_IsGrabbed()
{
	if (bEstaAgarrado)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue ,"Jugador Atrapado - Reproducir Animacion de Forcejeo");
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue ,"Jugador Liberado");
	}
}

void ACarreraDeObstaculosCharacter::ResetearCooldownEmpuje()
{
	bPuedeEmpujar= true;
}

void ACarreraDeObstaculosCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACarreraDeObstaculosCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ACarreraDeObstaculosCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACarreraDeObstaculosCharacter::Look);
	}
	else
	{
		UE_LOG(LogCarreraDeObstaculos, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACarreraDeObstaculosCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ACarreraDeObstaculosCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ACarreraDeObstaculosCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ACarreraDeObstaculosCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ACarreraDeObstaculosCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ACarreraDeObstaculosCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void ACarreraDeObstaculosCharacter::IntentarEmpujar()
{
	ACarreraGameState* MiGameState = GetWorld()->GetGameState<ACarreraGameState>();
	if (MiGameState&&!MiGameState->PuedeMoverse())return;
	if (bPuedeEmpujar)
	{
		Server_Empujar();
	}
}

void ACarreraDeObstaculosCharacter::ActualizarCheckpoint(FVector NuevaUbicacion)
{
	if (HasAuthority())
	{
		NuevaUbicacion.Z += 100.0f;
		UltimoCheckpoint = NuevaUbicacion;
	}
}

void ACarreraDeObstaculosCharacter::EjecutarRespawn()
{
	if (HasAuthority())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
		
		SetActorLocation(UltimoCheckpoint,false,nullptr,ETeleportType::ResetPhysics);
		
		Client_NotificarRespawn();
	}
}


void ACarreraDeObstaculosCharacter::Client_NotificarRespawn_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green ,"He Respawneado en mi Pantalla Local");
}

void ACarreraDeObstaculosCharacter::Multicast_EfectoEmpuje_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue ,"Empujaste a otro jugador");
}

void ACarreraDeObstaculosCharacter::Server_Empujar_Implementation()
{
	if (!bPuedeEmpujar) return;
	
	bPuedeEmpujar = false;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CooldownEmpuje, this, &ACarreraDeObstaculosCharacter::ResetearCooldownEmpuje, CooldownEmpuje, false);
	
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + (GetActorForwardVector() * 150.0f);
	
	FHitResult HitResult;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(50.0f);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	bool bHit =GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation,FQuat::Identity, ECC_Pawn, Sphere, QueryParams);
	if (bHit)
	{
		ACarreraDeObstaculosCharacter* OtroJugador= Cast<ACarreraDeObstaculosCharacter>(HitResult.GetActor());
		if (OtroJugador)
		{
			FVector DireccionEmpuje = GetActorForwardVector();
			DireccionEmpuje.Z = 0.5f;
			DireccionEmpuje.Normalize();
			
			OtroJugador->LaunchCharacter(DireccionEmpuje * FuerzaEmpuje, true, true);
		}
	}
	Multicast_EfectoEmpuje();
}
