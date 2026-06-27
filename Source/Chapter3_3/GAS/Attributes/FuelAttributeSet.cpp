// Fill out your copyright notice in the Description page of Project Settings.
#include "FuelAttributeSet.h"

UFuelAttributeSet::UFuelAttributeSet()
{
	InitFuel(100.f);
	InitMaxFuel(100.f);
}

void UFuelAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetFuelAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxFuel());
	}
}