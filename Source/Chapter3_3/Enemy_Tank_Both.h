// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Tank_Moving.h"
#include "Enemy_Tank_Both.generated.h"

UCLASS()
class CHAPTER3_3_API AEnemy_Tank_Both : public AEnemy_Tank_Moving
{
	GENERATED_BODY()
public:
	AEnemy_Tank_Both();

protected:
	virtual void Tick(float DeltaTime) override;
};