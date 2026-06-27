// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_FighterShoot.generated.h"

class AMyProjectile;

// 자원 소모 없이 즉시 발사
UCLASS()
class CHAPTER3_3_API UGA_FighterShoot : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_FighterShoot();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	TSubclassOf<AMyProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	float MuzzleOffset = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	float MuzzleZOffset = 20.f;
};