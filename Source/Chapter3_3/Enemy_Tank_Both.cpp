// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_Tank_Both.h"

void AEnemy_Tank_Both::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // PatrolMove 호출됨
	RotateTurretTowardPlayer(DeltaTime);
}