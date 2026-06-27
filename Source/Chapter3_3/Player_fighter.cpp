// Fill out your copyright notice in the Description page of Project Settings.

#include "Player_fighter.h"
#include "GAS/Attributes/HealthAttributeSet.h"
#include "GAS/Attributes/FuelAttributeSet.h"
#include "GAS/Abilities/GA_FighterShoot.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APlayer_fighter::APlayer_fighter()
{
	PrimaryActorTick.bCanEverTick = true;
	ShootAbilityClass = UGA_FighterShoot::StaticClass();

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(BoxComp);

	MuzzlePoint->SetupAttachment(MeshComp);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxComp);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	BoxComp->SetCollisionProfileName(TEXT("Pawn"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayer_fighter::BeginPlay()
{
	Super::BeginPlay();

	AddAttributeSet<UHealthAttributeSet>();
	AddAttributeSet<UFuelAttributeSet>();

	if (AbilitySystemComponent && ShootAbilityClass)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ShootAbilityClass, 1));
	}
}

void APlayer_fighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PC = Cast<AMyPlayerController>(GetController()))
		{
			if (PC->MoveAction)
			{
				EnhancedInput->BindAction(PC->MoveAction, ETriggerEvent::Triggered, this, &APlayer_fighter::Move);
				EnhancedInput->BindAction(PC->MoveAction, ETriggerEvent::Completed, this, &APlayer_fighter::StopMove);
			}
			if (PC->LookAction)
			{
				EnhancedInput->BindAction(PC->LookAction, ETriggerEvent::Triggered, this, &APlayer_fighter::Look);
			}
			// JumpAction(Space) = 상승
			if (PC->JumpAction)
			{
				EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Triggered, this, &APlayer_fighter::StartFlyUp);
				EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Completed, this, &APlayer_fighter::StopFlyUp);
			}
			// SprintAction(Shift) = 하강
			if (PC->SprintAction)
			{
				EnhancedInput->BindAction(PC->SprintAction, ETriggerEvent::Triggered, this, &APlayer_fighter::StartFlyDown);
				EnhancedInput->BindAction(PC->SprintAction, ETriggerEvent::Completed, this, &APlayer_fighter::StopFlyDown);
			}
		}
	}
}

void APlayer_fighter::Move(const FInputActionValue& Value)
{
	MoveInput = Value.Get<FVector2D>();
	TargetSpeed = MoveSpeed;
}

void APlayer_fighter::StopMove(const FInputActionValue& Value)
{
	MoveInput = FVector2D::ZeroVector;
	TargetSpeed = 0.f;
	CurrentSpeed = 0.f;
}

void APlayer_fighter::StartFlyUp(const FInputActionValue& Value)
{
	bIsAscending = true;
}

void APlayer_fighter::StopFlyUp(const FInputActionValue& Value)
{
	bIsAscending = false;
}

void APlayer_fighter::StartFlyDown(const FInputActionValue& Value)
{
	bIsDescending = true;
}

void APlayer_fighter::StopFlyDown(const FInputActionValue& Value)
{
	bIsDescending = false;
}

void APlayer_fighter::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	CurrentYaw += LookInput.X * TurnSpeed * DeltaTime;
	CurrentPitch -= LookInput.Y * TurnSpeed * DeltaTime;
	CurrentPitch = FMath::Clamp(CurrentPitch, -80.f, 80.f);

	SetActorRotation(FRotator(0.f, CurrentYaw, 0.f));

	if (SpringArm)
		SpringArm->SetRelativeRotation(FRotator(CurrentPitch, 0.f, 0.f));
}

void APlayer_fighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 수평 이동
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, AccelRate);
	if (!MoveInput.IsNearlyZero())
	{
		FVector Delta = FVector(MoveInput.X, MoveInput.Y, 0.f).GetSafeNormal();
		AddActorLocalOffset(Delta * CurrentSpeed * DeltaTime, true);
	}

	// 연료: 공중에서 시간에 따라 자동 소모, 착지 시 충전
	if (!bIsGrounded)
	{
		FuelGauge = FMath::Max(0.f, FuelGauge - FuelDrainRate * DeltaTime);
	}
	else
	{
		FuelGauge = FMath::Min(MaxFuel, FuelGauge + FuelRechargeRate * DeltaTime);
	}

	bool bHasFuel = FuelGauge > 0.f;

	// 수직 속도 결정
	if (bHasFuel)
	{
		// 연료 있음: 중력 없음
		if (bIsAscending)
			ZVelocity = AscendSpeed;
		else if (bIsDescending)
			ZVelocity = -DescendSpeed;
		else
			ZVelocity = 0.f; // 호버링 (제자리 유지)
	}
	else
	{
		// 연료 소진: 상승 불가, 활공
		if (bIsDescending)
			ZVelocity = -DescendSpeed;
		else
			ZVelocity += GlideGravity * DeltaTime;
	}

	// 수직 이동 적용 + 바닥 감지
	FHitResult Hit;
	AddActorLocalOffset(FVector(0.f, 0.f, ZVelocity * DeltaTime), true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
		ZVelocity = 0.f;
		bIsGrounded = true;
	}
	else
	{
		bIsGrounded = false;
	}
}