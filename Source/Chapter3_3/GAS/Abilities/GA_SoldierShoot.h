// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_SoldierShoot.generated.h"

class AMyProjectile;

// 탄약 체크 → 1발 소모 → 발사체 스폰
UCLASS()
class CHAPTER3_3_API UGA_SoldierShoot : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_SoldierShoot();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	TSubclassOf<AMyProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	TSubclassOf<UGameplayEffect> ConsumeAmmoEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	float MuzzleOffset = 120.f;

	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	float MuzzleZOffset = 50.f;
};