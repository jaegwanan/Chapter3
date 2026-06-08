// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Tank.h"
#include "Enemy_Tank_Turret.generated.h"

UCLASS()
class CHAPTER3_3_API AEnemy_Tank_Turret : public AEnemy_Tank
{
	GENERATED_BODY()

protected:
	virtual void Tick(float DeltaTime) override;
};