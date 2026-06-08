// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy_Tank.generated.h"

UCLASS()
class CHAPTER3_3_API AEnemy_Tank : public AActor
{
	GENERATED_BODY()

public:
	AEnemy_Tank();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BarrelMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
	float TurretRotationSpeed = 90.f;

	// 메시 앞방향이 X축과 다를 때 보정값 (예: Y축이면 -90, -X면 180)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
	float TurretYawOffset = 0.f;

	void RotateTurret(float DeltaTime);
	void RotateTurretTowardPlayer(float DeltaTime);
};
