// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Player_tank.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

UCLASS()
class CHAPTER3_3_API APlayer_tank : public APawn
{
	GENERATED_BODY()

public:
	APlayer_tank();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComp;

	// 탱크 차체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BodyMesh;

	// 포탑 (차체 기준 좌우 회전)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* TurretMesh;

	// 포신 (포탑 기준 상하 회전)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION() void Move(const FInputActionValue& Value);
	UFUNCTION() void StopMove(const FInputActionValue& Value);
	UFUNCTION() void Look(const FInputActionValue& Value);

	// 이동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 400.f;

	// A/D 차체 회전 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float HullTurnSpeed = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AccelRate = 5.f;

	// 포탑/포신 회전 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float TurretTurnSpeed = 120.f;

	// 포신 상하 각도 제한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float GunPitchMin = -10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float GunPitchMax = 20.f;

	FVector2D MoveInput = FVector2D::ZeroVector;
	float TargetSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	float CurrentSpeed = 0.f;

	float TurretYaw = 0.f;
	float GunPitch = 0.f;

	// 포탑 메시 기본 방향 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float TurretMeshYawOffset = -90.f;

	// 포신 메시 기본 방향 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float GunMeshYawOffset = 0.f;

	// 중력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Gravity = -980.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsGrounded = false;

	float ZVelocity = 0.f;
};