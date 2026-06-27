// Fill out your copyright notice in the Description page of Project Settings.

#include "Player_tank.h"
#include "GAS/Attributes/HealthAttributeSet.h"
#include "GAS/Abilities/GA_TankShoot.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APlayer_tank::APlayer_tank()
{
	PrimaryActorTick.bCanEverTick = true;
	ShootAbilityClass = UGA_TankShoot::StaticClass();

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(BoxComp);
	BoxComp->SetCollisionProfileName(TEXT("Pawn"));

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxComp);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BoxComp);
	TurretMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(TurretMesh);
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MuzzlePoint->SetupAttachment(GunMesh); // 포신 끝에서 발사

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxComp);
	SpringArm->TargetArmLength = 500.f;
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;
}

void APlayer_tank::BeginPlay()
{
	Super::BeginPlay();

	AddAttributeSet<UHealthAttributeSet>();

	if (AbilitySystemComponent && ShootAbilityClass)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ShootAbilityClass, 1));
	}
}

void APlayer_tank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PC = Cast<AMyPlayerController>(GetController()))
		{
			if (PC->MoveAction)
			{
				EnhancedInput->BindAction(PC->MoveAction, ETriggerEvent::Triggered, this, &APlayer_tank::Move);
				EnhancedInput->BindAction(PC->MoveAction, ETriggerEvent::Completed, this, &APlayer_tank::StopMove);
			}
			if (PC->LookAction)
			{
				EnhancedInput->BindAction(PC->LookAction, ETriggerEvent::Triggered, this, &APlayer_tank::Look);
			}
		}
	}
}

void APlayer_tank::Move(const FInputActionValue& Value)
{
	MoveInput = Value.Get<FVector2D>();
	TargetSpeed = MoveSpeed;
}

void APlayer_tank::StopMove(const FInputActionValue& Value)
{
	MoveInput = FVector2D::ZeroVector;
	TargetSpeed = 0.f;
	CurrentSpeed = 0.f;
}

void APlayer_tank::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	// 포탑 좌우 회전
	TurretYaw += LookInput.X * TurretTurnSpeed * DeltaTime;

	// 포신 상하 회전
	GunPitch -= LookInput.Y * TurretTurnSpeed * DeltaTime;
	GunPitch = FMath::Clamp(GunPitch, GunPitchMin, GunPitchMax);

	if (TurretMesh)
		TurretMesh->SetRelativeRotation(FRotator(0.f, TurretYaw + TurretMeshYawOffset, 0.f));

	if (GunMesh)
		GunMesh->SetRelativeRotation(FRotator(GunPitch, GunMeshYawOffset, 0.f));

	// 카메라도 포탑+포신 방향 따라감
	if (SpringArm)
		SpringArm->SetRelativeRotation(FRotator(GunPitch, TurretYaw, 0.f));
}

void APlayer_tank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, AccelRate);

	// W/S: 전진/후진
	if (FMath::Abs(MoveInput.X) > 0.f)
	{
		AddActorLocalOffset(FVector(MoveInput.X * CurrentSpeed * DeltaTime, 0.f, 0.f), true);
	}

	// A/D: 차체 좌우 회전
	if (FMath::Abs(MoveInput.Y) > 0.f)
	{
		AddActorLocalRotation(FRotator(0.f, MoveInput.Y * HullTurnSpeed * DeltaTime, 0.f));
	}

	// 중력
	ZVelocity += Gravity * DeltaTime;

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