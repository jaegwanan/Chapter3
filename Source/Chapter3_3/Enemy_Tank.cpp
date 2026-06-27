// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Tank.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

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

	MuzzlePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(BarrelMesh);
	MuzzlePoint->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	MuzzlePoint->bHiddenInGame = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyAsset(TEXT("/Game/Military_Free/Meshes/SM_tank_001"));
	if (BodyAsset.Succeeded()) BodyMesh->SetStaticMesh(BodyAsset.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TurretAsset(TEXT("/Game/Military_Free/Meshes/SM_tank_tower_001"));
	if (TurretAsset.Succeeded()) TurretMesh->SetStaticMesh(TurretAsset.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BarrelAsset(TEXT("/Game/Military_Free/Meshes/SM_tank_gun_001"));
	if (BarrelAsset.Succeeded()) BarrelMesh->SetStaticMesh(BarrelAsset.Object);
}

void AEnemy_Tank::BeginPlay()
{
	Super::BeginPlay();

	if (bCanShoot && ProjectileClass && ShootInterval > 0.f)
	{
		GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &AEnemy_Tank::Shoot, ShootInterval, true, ShootInterval);
	}
}

float AEnemy_Tank::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (bDead) return 0.f;
	bDead = true;

	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
	Destroy();
	return DamageAmount;
}

void AEnemy_Tank::Destroyed()
{
	Super::Destroyed();

	// WaveGameMode에 킬 이벤트 전달 (OnEnemyKilled는 UFUNCTION이어야 함)
	if (AGameModeBase* GM = UGameplayStatics::GetGameMode(this))
	{
		if (UFunction* Func = GM->FindFunction(TEXT("OnEnemyKilled")))
		{
			GM->ProcessEvent(Func, nullptr);
		}
	}
}

void AEnemy_Tank::Shoot()
{
	if (!ProjectileClass) return;

	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) return;

	FVector MuzzleLoc = MuzzlePoint->GetComponentLocation();
	FVector ToPlayer = (Player->GetActorLocation() - MuzzleLoc).GetSafeNormal();
	FRotator ShootRot = ToPlayer.Rotation();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMyProjectile* Proj = GetWorld()->SpawnActorDeferred<AMyProjectile>(
		ProjectileClass, FTransform(ShootRot, MuzzleLoc), this, nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (Proj)
	{
		Proj->Damage = ShootDamage;
		Proj->FinishSpawning(FTransform(ShootRot, MuzzleLoc));
	}
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


