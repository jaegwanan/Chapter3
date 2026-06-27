// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_ConsumeAmmo.generated.h"

// Instant 효과: Ammo 속성을 -1 감소
UCLASS()
class CHAPTER3_3_API UGE_ConsumeAmmo : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_ConsumeAmmo();
};