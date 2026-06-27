// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WaveGameMode.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart  UMETA(DisplayName = "Waiting To Start"),
	InProgress      UMETA(DisplayName = "In Progress"),
	WaveComplete    UMETA(DisplayName = "Wave Complete"),
	WaveFailed      UMETA(DisplayName = "Wave Failed"),
};

USTRUCT(BlueprintType)
struct FWaveConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float TimeLimit = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 CoinTarget = 0;

	// 0이면 킬 조건 없음, 양수면 해당 수만큼 처치 후 목표지점 진입 가능
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 KillTarget = 0;

	// 클리어 후 이동할 다음 레벨 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	FName NextLevelName = NAME_None;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveStateChanged, EWaveState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCoinCountChanged, int32, Collected, int32, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerUpdated, float, RemainingSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKillCountChanged, int32, Killed, int32, Target);

UCLASS()
class CHAPTER3_3_API AWaveGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AWaveGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave|Config")
	FWaveConfig WaveConfig;

	// 런타임 상태 (UI에서 읽기용)
	UPROPERTY(BlueprintReadOnly, Category = "Wave|State")
	float RemainingTime = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Wave|State")
	int32 CollectedCoins = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Wave|State")
	int32 KilledEnemies = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Wave|State")
	EWaveState WaveState = EWaveState::WaitingToStart;

	// HUD/Widget 블루프린트에서 바인딩할 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnWaveStateChanged OnWaveStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnCoinCountChanged OnCoinCountChanged;

	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnTimerUpdated OnTimerUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnKillCountChanged OnKillCountChanged;

	// 게임 액터에서 호출 (CoinActor, GoalZone, MyGASPawn, Enemy_Tank)
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void OnCoinCollected();

	// Enemy_Tank::Destroyed에서 ProcessEvent로 호출 — UFUNCTION 필수
	UFUNCTION()
	void OnEnemyKilled();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void OnPlayerReachedGoal();

	// MyGASPawn에서 ProcessEvent로 호출하기 때문에 UFUNCTION 필수
	UFUNCTION()
	void OnPlayerDied();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	float GetRemainingTime() const { return RemainingTime; }

	UFUNCTION(BlueprintCallable, Category = "Wave")
	int32 GetCollectedCoins() const { return CollectedCoins; }

	UFUNCTION(BlueprintCallable, Category = "Wave")
	int32 GetCoinTarget() const { return WaveConfig.CoinTarget; }

	UFUNCTION(BlueprintCallable, Category = "Wave")
	EWaveState GetWaveState() const { return WaveState; }

	// 위젯에서 어떤 카운터를 표시할지 판단할 때 사용
	UFUNCTION(BlueprintCallable, Category = "Wave")
	bool HasCoinTarget() const { return WaveConfig.CoinTarget > 0; }

	UFUNCTION(BlueprintCallable, Category = "Wave")
	bool HasKillTarget() const { return WaveConfig.KillTarget > 0; }

	// 다음 레벨이 없으면 최종 웨이브 → "GAME CLEAR!" 표시용
	UFUNCTION(BlueprintCallable, Category = "Wave")
	bool IsFinalWave() const { return WaveConfig.NextLevelName.IsNone(); }

	// WaveFailed 시 재시작 버튼에서 호출
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void RestartLevel();

protected:
	virtual void BeginPlay() override;

	FTimerHandle CountdownTimerHandle;
	bool bGoalReached = false;

	void StartWave();
	void TickCountdown();
	void SetWaveState(EWaveState NewState);
	void EndWave(bool bSuccess);
};