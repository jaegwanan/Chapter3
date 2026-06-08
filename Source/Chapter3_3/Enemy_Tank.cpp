// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Tank.h"
#include "Kismet/GameplayStatics.h"

AEnemy_Tank::AEnemy_Tank()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BodyMesh->SetupAttachment(SceneRoot);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel"));
	BarrelMesh->SetupAttachment(TurretMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyAsset(TEXT("/Game/Military_Free/Meshes/SM_tank_001"));
	if (BodyAsset.Succeeded()) BodyMesh->SetStaticMesh(BodyAsset.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TurretAsset(TEXT("/Game/Military_Free/Meshes/SM_tank_tower_001"));
	if (TurretAsset.Succeeded()) TurretMesh->SetStaticMesh(TurretAsset.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrelAsset(TEXT("/Game/Military_Free/Meshes/SM_tank_gun_001"));
	if (BarrelAsset.Succeeded()) BarrelMesh->SetStaticMesh(BarrelAsset.Object);
}

void AEnemy_Tank::RotateTurret(float DeltaTime)
{
	TurretMesh->AddRelativeRotation(FRotator(0.f, TurretRotationSpeed * DeltaTime, 0.f));
}

void AEnemy_Tank::RotateTurretTowardPlayer(float DeltaTime)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) return;

	FVector ToPlayer = Player->GetActorLocation() - TurretMesh->GetComponentLocation();
	FRotator TurretTarget(0.f, ToPlayer.Rotation().Yaw + TurretYawOffset, 0.f);
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(TurretMesh->GetComponentRotation(), TurretTarget, DeltaTime, TurretRotationSpeed)
	);

	FVector LocalToPlayer = TurretMesh->GetComponentTransform().InverseTransformPosition(Player->GetActorLocation());
	float Pitch = FMath::RadiansToDegrees(
		FMath::Atan2(LocalToPlayer.Z, FVector2D(LocalToPlayer.X, LocalToPlayer.Y).Size())
	);
	BarrelMesh->SetRelativeRotation(
		FMath::RInterpTo(BarrelMesh->GetRelativeRotation(), FRotator(Pitch, 0.f, 0.f), DeltaTime, TurretRotationSpeed)
	);
}


