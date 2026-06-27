// Fill out your copyright notice in the Description page of Project Settings.
#include "AmmoAttributeSet.h"

UAmmoAttributeSet::UAmmoAttributeSet()
{
	InitAmmo(30.f);
	InitMaxAmmo(30.f);
}

void UAmmoAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxAmmo());
	}
}