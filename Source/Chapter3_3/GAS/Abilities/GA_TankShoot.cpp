// Fill out your copyright notice in the Description page of Project Settings.
#include "GA_TankShoot.h"
#include "GAS/MyGASPawn.h"
#include "MyProjectile.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"

UGA_TankShoot::UGA_TankShoot()
{
	// 상태(LastFireTime)를 멤버에 보관해야 하므로 인스턴싱 필요
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_TankShoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 쿨다운 체크
	const float Now = GetWorld()->GetTimeSeconds();
	if (Now - LastFireTime < CooldownDuration)
	{
		UE_LOG(LogTemp, Warning, TEXT("TankShoot: 재장전 중 (%.1f초 남음)"),
			CooldownDuration - (Now - LastFireTime));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	LastFireTime = Now;

	// 발사체 스폰 - 카메라(포탑+포신) 방향으로
	APawn* OwnerPawn = Cast<APawn>(ActorInfo->AvatarActor.Get());
	if (OwnerPawn && ProjectileClass)
	{
		UCameraComponent* Camera = OwnerPawn->FindComponentByClass<UCameraComponent>();
		FRotator SpawnRot = Camera ? Camera->GetComponentRotation() : OwnerPawn->GetActorRotation();
		FVector AimDir = SpawnRot.Vector();

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