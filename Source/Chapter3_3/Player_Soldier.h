// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GAS/MyGASPawn.h"
#include "Player_Soldier.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

UCLASS()
class CHAPTER3_3_API APlayer_Soldier : public AMyGASPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayer_Soldier();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
	UFUNCTION()
	void StartSprint(const FInputActionValue& Value);

	UFUNCTION()
	void StopSprint(const FInputActionValue& Value);

	UFUNCTION()
	void StopMove(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MoveSpeed = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float WalkSpeed = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float SprintSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float TurnSpeed = 200.f;
	
	float CurrentYaw = 0.f;
	float CurrentPitch = 0.f;

	// 가속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float AccelRate = 5.f;

	// 현재 프레임의 입력 방향 (Move에서 저장, Tick에서 소비)
	FVector2D MoveInput = FVector2D::ZeroVector;

	// Tick에서 CurrentSpeed가 보간해 나갈 목표 속도
	float TargetSpeed = 0.f;

	// ABP에서 읽는 애니메이션 구동 변수
	UPROPERTY(BlueprintReadOnly, Category="Anim")
	float CurrentSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category="Anim")
	bool bIsSprinting = false;

	UPROPERTY(BlueprintReadOnly, Category="Anim")
	bool bIsGrounded = false;

	UPROPERTY(BlueprintReadOnly, Category="Anim")
	float ZVelocity = 0.f;
	
	// 중력 가속도 (cm/s²).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float Gravity = -980.f;
	
	// 점프 시작 시 위쪽으로 주는 초기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float JumpVelocity = 600.f;
};
