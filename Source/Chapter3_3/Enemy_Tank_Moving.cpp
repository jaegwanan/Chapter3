// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_Tank_Moving.h"

AEnemy_Tank_Moving::AEnemy_Tank_Moving()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemy_Tank_Moving::BeginPlay()
{
	Super::BeginPlay();

	FVector Forward = GetActorForwardVector();
	PatrolStartPoint = GetActorLocation() - Forward * (PatrolLength * 0.5f);
	PatrolEndPoint   = GetActorLocation() + Forward * (PatrolLength * 0.5f);
}

void AEnemy_Tank_Moving::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PatrolMove(DeltaTime);
}

void AEnemy_Tank_Moving::PatrolMove(float DeltaTime)
{
	FVector Target = bMovingToEnd ? PatrolEndPoint : PatrolStartPoint;
	FVector ToTarget = Target - GetActorLocation();

	if (ToTarget.SizeSquared() < 50.f * 50.f)
	{
		bMovingToEnd = !bMovingToEnd;
		return;
	}

	FVector Direction = ToTarget.GetSafeNormal();
	AddActorWorldOffset(Direction * MoveSpeed * DeltaTime, true);

	// 차체가 이동 방향으로 서서히 회전
	FRotator TargetRot(0.f, Direction.Rotation().Yaw, 0.f);
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 5.f));
}