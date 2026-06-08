// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_Tank_Turret.h"

void AEnemy_Tank_Turret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateTurret(DeltaTime);
}