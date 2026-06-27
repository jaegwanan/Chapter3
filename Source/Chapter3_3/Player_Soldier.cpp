// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Soldier.h"
#include "GAS/Attributes/HealthAttributeSet.h"
#include "GAS/Attributes/AmmoAttributeSet.h"
#include "GAS/Abilities/GA_SoldierShoot.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APlayer_Soldier::APlayer_Soldier()
{
	PrimaryActorTick.bCanEverTick = true;
	ShootAbilityClass = UGA_SoldierShoot::StaticClass();
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComp);
	
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(CapsuleComp);

	MuzzlePoint->SetupAttachment(MeshComp);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CapsuleComp);
	SpringArm->TargetArmLength = 300.0f;  
	SpringArm->bUsePawnControlRotation = false; 
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	CapsuleComp->SetCollisionProfileName(TEXT("Pawn"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayer_Soldier::BeginPlay()
{
	Super::BeginPlay();

	AddAttributeSet<UHealthAttributeSet>();
	AddAttributeSet<UAmmoAttributeSet>();

	if (AbilitySystemComponent && ShootAbilityClass)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ShootAbilityClass, 1));
	}
}

void APlayer_Soldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APlayer_Soldier::Move
                );
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Completed,
                    this,
                    &APlayer_Soldier::StopMove
                );
            }
            
            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APlayer_Soldier::StartJump
                );

                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &APlayer_Soldier::StopJump
                );
            }
            
            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APlayer_Soldier::Look
                );
            }
            
            if (PlayerController->SprintAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered, 
                    this, 
                    &APlayer_Soldier::StartSprint
                );
                EnhancedInput->BindAction(
                    PlayerController->SprintAction, 
                    ETriggerEvent::Completed, 
                    this, 
                    &APlayer_Soldier::StopSprint
                );
            }    
        }
    }
}

void APlayer_Soldier::Move(const FInputActionValue& Value)
{
	// 방향만 저장. 실제 이동은 Tick에서 처리
	MoveInput = Value.Get<FVector2D>();
	TargetSpeed = MoveSpeed;
}

void APlayer_Soldier::StopMove(const FInputActionValue& Value)
{
	MoveInput = FVector2D::ZeroVector;
	TargetSpeed = 0.f;
	CurrentSpeed = 0.f;
}

void APlayer_Soldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, AccelRate);

	// 수평 이동: 입력 방향 × 보간된 속도
	if (!MoveInput.IsNearlyZero())
	{
		FVector Delta = FVector(MoveInput.X, MoveInput.Y, 0.f).GetSafeNormal();
		AddActorLocalOffset(Delta * CurrentSpeed * DeltaTime, true);
	}

	// 중력 누적 및 수직 이동
	ZVelocity += Gravity * DeltaTime;

	FHitResult Hit;
	AddActorLocalOffset(FVector(0.f, 0.f, ZVelocity * DeltaTime), true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
		if (ZVelocity < 0.f)
			bIsGrounded = true;
		ZVelocity = 0.f;
	}
	else
	{
		bIsGrounded = false;
	}
}

void APlayer_Soldier::StartJump(const FInputActionValue& Value)
{
	if (bIsGrounded)
	{
		ZVelocity = JumpVelocity;
		bIsGrounded = false;
	}
}

void APlayer_Soldier::StopJump(const FInputActionValue& Value)
{
    // 슈퍼 점프 기능, (길게 누르면 더 높게 점프)
	if (ZVelocity > 0.f)
	{
		ZVelocity *= 0.5f;
	}
}

void APlayer_Soldier::StartSprint(const FInputActionValue& Value)
{
	MoveSpeed = SprintSpeed;
	bIsSprinting = true;
}

void APlayer_Soldier::StopSprint(const FInputActionValue& Value)
{
	MoveSpeed = WalkSpeed;
	bIsSprinting = false;
}

void APlayer_Soldier::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	CurrentYaw += LookInput.X * TurnSpeed * DeltaTime;
	CurrentPitch -= LookInput.Y * TurnSpeed * DeltaTime;
	
	CurrentPitch = FMath::Clamp(CurrentPitch, -15.0f, 15.0f); //제한

	SetActorRotation(FRotator(0.f, CurrentYaw, 0.f));
	
	if (SpringArm)
	{
		SpringArm->SetRelativeRotation(FRotator(CurrentPitch, 0.f, 0.f));
	}
	// AddActorLocalRotation 대신 SetActorRotation 사용. 위아래 제한을 두고싶어서 그랬습니다.
}