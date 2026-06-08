// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Tank.h"
#include "Enemy_Tank_Moving.generated.h"

UCLASS()
class CHAPTER3_3_API AEnemy_Tank_Moving : public AEnemy_Tank
{
	GENERATED_BODY()

public:
	AEnemy_Tank_Moving();

protected:
	// 스폰 위치 기준 앞뒤로 이동할 총 거리 (절반씩 양방향)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Patrol")
	float PatrolLength = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Patrol")
	float MoveSpeed = 300.f;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void PatrolMove(float DeltaTime);

private:
	FVector PatrolStartPoint;
	FVector PatrolEndPoint;
	bool bMovingToEnd = true;
};