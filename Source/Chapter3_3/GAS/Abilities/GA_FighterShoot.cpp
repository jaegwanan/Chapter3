// Fill out your copyright notice in the Description page of Project Settings.
#include "GA_FighterShoot.h"
#include "GAS/MyGASPawn.h"
#include "MyProjectile.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"

UGA_FighterShoot::UGA_FighterShoot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_FighterShoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

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