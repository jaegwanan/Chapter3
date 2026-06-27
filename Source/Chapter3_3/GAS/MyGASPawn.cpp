// Fill out your copyright notice in the Description page of Project Settings.
#include "MyGASPawn.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "HealthAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

AMyGASPawn::AMyGASPawn()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	MuzzlePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->ArrowColor = FColor::Red;
	MuzzlePoint->ArrowSize = 0.5f;
	MuzzlePoint->bHiddenInGame = true;
	// 실제 부모 컴포넌트는 각 서브클래스 생성자에서 붙임
}

UAbilitySystemComponent* AMyGASPawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMyGASPawn::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AMyGASPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PC = Cast<AMyPlayerController>(GetController()))
		{
			if (PC->ShootAction && ShootAbilityClass)
			{
				EIC->BindAction(PC->ShootAction, ETriggerEvent::Triggered, this, &AMyGASPawn::OnShoot);
			}
		}
	}
}

float AMyGASPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float Actual = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (!AbilitySystemComponent || DamageAmount <= 0.f) return Actual;

	const UHealthAttributeSet* HealthSet = AbilitySystemComponent->GetSet<UHealthAttributeSet>();
	if (!HealthSet) return Actual;

	float NewHealth = FMath::Max(0.f, HealthSet->GetHealth() - DamageAmount);
	AbilitySystemComponent->SetNumericAttributeBase(UHealthAttributeSet::GetHealthAttribute(), NewHealth);

	if (NewHealth <= 0.f)
	{
	
		if (AGameModeBase* GM = UGameplayStatics::GetGameMode(this))
		{
			// WaveGameMode가 존재 시 사망 이벤트 전달
			if (UFunction* Func = GM->FindFunction(TEXT("OnPlayerDied")))
			{
				GM->ProcessEvent(Func, nullptr);
			}
		}
	}

	return DamageAmount;
}

void AMyGASPawn::OnShoot(const FInputActionValue& Value)
{
	if (AbilitySystemComponent && ShootAbilityClass)
	{
		AbilitySystemComponent->TryActivateAbilityByClass(ShootAbilityClass);
	}
}