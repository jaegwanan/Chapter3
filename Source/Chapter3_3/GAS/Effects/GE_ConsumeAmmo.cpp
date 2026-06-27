// Fill out your copyright notice in the Description page of Project Settings.
#include "GE_ConsumeAmmo.h"
#include "GAS/Attributes/AmmoAttributeSet.h"

UGE_ConsumeAmmo::UGE_ConsumeAmmo()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Modifier;
	Modifier.Attribute = UAmmoAttributeSet::GetAmmoAttribute();
	Modifier.ModifierOp = EGameplayModOp::Additive;
	Modifier.ModifierMagnitude = FScalableFloat(-1.f);
	Modifiers.Add(Modifier);
}