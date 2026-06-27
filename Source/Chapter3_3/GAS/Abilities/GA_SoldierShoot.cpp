// Fill out your copyright notice in the Description page of Project Settings.
#include "GA_SoldierShoot.h"
#include "GAS/Attributes/AmmoAttributeSet.h"
#include "GAS/Effects/GE_ConsumeAmmo.h"
#include "GAS/MyGASPawn.h"
#include "MyProjectile.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"

UGA_SoldierShoot::UGA_SoldierShoot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ConsumeAmmoEffect = UGE_ConsumeAmmo::StaticClass();
}

void UGA_SoldierShoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 탄약 확인
	const UAmmoAttributeSet* AmmoSet = ASC->GetSet<UAmmoAttributeSet>();
	if (!AmmoSet || AmmoSet->GetAmmo() <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("SoldierShoot: 탄약 없음"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 탄약 1발 소모
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ConsumeAmmoEffect, 1.f, EffectContext);
	if (SpecHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}

	// 발사체 스폰 - 카메라 방향으로
	APawn* OwnerPawn = Cast<APawn>(ActorInfo->AvatarActor.Get());
	if (OwnerPawn && ProjectileClass)
	{
		UCameraComponent* Camera = OwnerPawn->FindComponentByClass<UCameraComponent>();
		FRotator SpawnRot = Camera ? Camera->GetComponentRotation() : OwnerPawn->GetActorRotation();
		FVector AimDir = SpawnRot.Vector();

		// MuzzlePoint가 있으면 그 위치 사용, 없으면 오프셋 계산
		AMyGASPawn* GASPawn = Cast<AMyGASPawn>(OwnerPawn);
		FVector SpawnLoc = (GASPawn && GASPawn->MuzzlePoint)
			? GASPawn->MuzzlePoint->GetComponentLocation()
			: OwnerPawn->GetActorLocation() + FVector(0.f, 0.f, MuzzleZOffset) + AimDir * MuzzleOffset;

		FActorSpawnParameters Params;
		Params.Owner = OwnerPawn;
		Params.Instigator = OwnerPawn;
		GetWorld()->SpawnActor<AMyProjectile>(ProjectileClass, SpawnLoc, SpawnRot, Params);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}