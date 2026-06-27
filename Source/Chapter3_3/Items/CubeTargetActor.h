// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeTargetActor.generated.h"

// 레벨 3용 사격 표적 큐브 — 맞으면 파괴되고 WaveGameMode 킬 카운트에 집계됨
UCLASS()
class CHAPTER3_3_API ACubeTargetActor : public AActor
{
	GENERATED_BODY()
public:
	ACubeTargetActor();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void Destroyed() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cube")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cube")
	class UBoxComponent* CollisionBox;

	bool bDestroyed = false;
};