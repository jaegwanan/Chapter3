// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Wave/WaveGameMode.h"
#include "WaveHUDWidget.generated.h"

// Blueprint 자식(WBP_WaveHUD)에서 각 함수를 오버라이드해 TextBlock 등을 업데이트
UCLASS()
class CHAPTER3_3_API UWaveHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 1초마다 호출 — 남은 시간(초)을 MM:SS 형식 등으로 표시
	UFUNCTION(BlueprintImplementableEvent, Category = "WaveHUD")
	void UpdateTimer(float RemainingSeconds);

	// 코인 수집 시 호출 — "3 / 10" 형식으로 표시
	UFUNCTION(BlueprintImplementableEvent, Category = "WaveHUD")
	void UpdateCoins(int32 Collected, int32 Target);

	// 웨이브 상태 변경 시 호출 — CLEAR / FAILED 메시지 표시
	UFUNCTION(BlueprintImplementableEvent, Category = "WaveHUD")
	void ShowWaveResult(EWaveState State);

	// 체력 변경 시 호출 — ProgressBar percent = CurrentHealth / MaxHealth
	UFUNCTION(BlueprintImplementableEvent, Category = "WaveHUD")
	void UpdateHealth(float CurrentHealth, float MaxHealth);

	// 킬 카운트 변경 시 호출 — "2 / 5" 형식
	UFUNCTION(BlueprintImplementableEvent, Category = "WaveHUD")
	void UpdateKillCount(int32 Killed, int32 Target);
};