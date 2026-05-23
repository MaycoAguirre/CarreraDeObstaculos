// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CarreraDeObstaculosCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class ACarreraDeObstaculosCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

public:

	/** Constructor */
	ACarreraDeObstaculosCharacter();	
	
	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();
	
	UFUNCTION(BlueprintCallable, Category="Mecanicas")
	void IntentarAgarrar();
	
	UFUNCTION(BlueprintCallable, Category="Mecanicas")
	void IntentarEmpujar();
	
	UFUNCTION(Server, Reliable)
	void Server_Empujar();
	
	UFUNCTION(Server, Reliable)
	void Server_Agarrar();
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_EfectoAgarre();
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_EfectoEmpuje();
	
	UPROPERTY(ReplicatedUsing="OnRep_IsGrabbed", BlueprintReadOnly, Category="Estado")
	bool bEstaAgarrado;
	
	UFUNCTION()
	void OnRep_IsGrabbed();
	
	UFUNCTION(BlueprintCallable, Category="Efectos")
	void AplicarEfectoAgarre(ACarreraDeObstaculosCharacter* Atacante, float Duracion);
	
	virtual void Jump() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mecanicas")
	float CooldownAgarre = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mecanicas")
	float DuracionAgarre = 1.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mecanicas")
	float CooldownEmpuje = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mecanicas")
	float FuerzaEmpuje = 1500.0f;
	
	void ActualizarCheckpoint(FVector NuevaUbicacion);
	
	void EjecutarRespawn();
	
	UFUNCTION(Client, Reliable)
	void Client_NotificarRespawn();
	
	
	
	

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
protected:
	virtual void BeginPlay() override;
	
	bool bPuedeAgarrar = true;
	FTimerHandle TimerHandle_CooldownAgarre;
	FTimerHandle TimerHandle_Liberacion;
	
	float VelocidadOriginal;
	
	void ResetearCooldownAgarre();
	void FinalizarAgarre();
	
	FVector UltimoCheckpoint;

	bool bPuedeEmpujar = true;
	FTimerHandle TimerHandle_CooldownEmpuje;
	
	void ResetearCooldownEmpuje();
	
	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;
	
};

