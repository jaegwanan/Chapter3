// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "MyProjectile.h"
#include "Enemy_Tank.generated.h"

UCLASS()
class CHAPTER3_3_API AEnemy_Tank : public AActor
{
	GENERATED_BODY()

public:
	AEnemy_Tank();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BarrelMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* MuzzlePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
	float TurretRotationSpeed = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
	float TurretYawOffset = 0.f;

	// 발사 활성화 여부 — Both 서브클래스에서 true로 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shoot")
	bool bCanShoot = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shoot")
	float ShootInterval = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shoot")
	float ShootDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shoot")
	TSubclassOf<AMyProjectile> ProjectileClass;

	FTimerHandle ShootTimerHandle;
	bool bDead = false;

	void Shoot();
	void RotateTurret(float DeltaTime);
	void RotateTurretTowardPlayer(float DeltaTime);
};
