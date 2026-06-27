// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_TankShoot.generated.h"

class AMyProjectile;

// 쿨다운(2초) 후 발사, 탄약 소모 없음
UCLASS()
class CHAPTER3_3_API UGA_TankShoot : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_TankShoot();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	TSubclassOf<AMyProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	float CooldownDuration = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	float MuzzleOffset = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	float MuzzleZOffset = 50.f;

private:
	float LastFireTime = -10.f;
};