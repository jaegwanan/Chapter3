// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Wave/WaveGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameplayEffectTypes.h"
#include "WaveHUD.generated.h"

class UWaveHUDWidget;
class UAbilitySystemComponent;

UCLASS()
class CHAPTER3_3_API AWaveHUD : public AHUD
{
	GENERATED_BODY()
public:
	AWaveHUD();

protected:
	virtual void BeginPlay() override;

	// 블루프린트 서브클래스(BP_WaveHUD)의 디테일에서 WBP_WaveHUD 연결
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UWaveHUDWidget> HUDWidgetClass;

	UPROPERTY()
	UWaveHUDWidget* HUDWidget;

	// 체력 변경 콜백 바인딩용 ASC 레퍼런스
	UPROPERTY()
	UAbilitySystemComponent* BoundASC;

	UFUNCTION()
	void HandleTimerUpdated(float RemainingSeconds);

	UFUNCTION()
	void HandleCoinCountChanged(int32 Collected, int32 Target);

	UFUNCTION()
	void HandleWaveStateChanged(EWaveState NewState);

	UFUNCTION()
	void HandleKillCountChanged(int32 Killed, int32 Target);

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	void BindHealthDelegate();
};