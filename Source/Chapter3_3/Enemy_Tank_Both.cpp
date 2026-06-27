// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_Tank_Both.h"

AEnemy_Tank_Both::AEnemy_Tank_Both()
{
	// 이동 + 조준 + 사격 모두 활성화
	bCanShoot = true;
}

void AEnemy_Tank_Both::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // PatrolMove 호출됨
	RotateTurretTowardPlayer(DeltaTime);
}