// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Player_fighter.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

UCLASS()
class CHAPTER3_3_API APlayer_fighter : public APawn
{
	GENERATED_BODY()

public:
	APlayer_fighter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION() void Move(const FInputActionValue& Value);
	UFUNCTION() void StopMove(const FInputActionValue& Value);
	UFUNCTION() void Look(const FInputActionValue& Value);
	UFUNCTION() void StartFlyUp(const FInputActionValue& Value);
	UFUNCTION() void StopFlyUp(const FInputActionValue& Value);
	UFUNCTION() void StartFlyDown(const FInputActionValue& Value);
	UFUNCTION() void StopFlyDown(const FInputActionValue& Value);

	// 수평 이동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AccelRate = 5.f;

	FVector2D MoveInput = FVector2D::ZeroVector;
	float TargetSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	float CurrentSpeed = 0.f;

	float CurrentYaw = 0.f;
	float CurrentPitch = 0.f;

	// 수직 이동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float AscendSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float DescendSpeed = 400.f;

	// 게이지 소진 시 낙하 중력 (활공 느낌, 약하게)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float GlideGravity = -200.f;

	// 게이지 있을 때 일반 중력 (Shift/Space 안 누를 때)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float NormalGravity = -980.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float MaxFuel = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = "Flight")
	float FuelGauge = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float FuelDrainRate = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float FuelRechargeRate = 10.f;

	UPROPERTY(BlueprintReadOnly, Category = "Flight")
	bool bIsGrounded = false;

	float ZVelocity = 0.f;
	bool bIsAscending = false;
	bool bIsDescending = false;
};
