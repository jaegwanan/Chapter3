// Fill out your copyright notice in the Description page of Project Settings.
#include "WaveGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AWaveGameMode::AWaveGameMode()
{
	WaveConfig.TimeLimit = 60.0f;
	WaveConfig.CoinTarget = 10;
}

void AWaveGameMode::BeginPlay()
{
	Super::BeginPlay();
	StartWave();
}

void AWaveGameMode::StartWave()
{
	CollectedCoins = 0;
	KilledEnemies = 0;
	bGoalReached = false;
	RemainingTime = WaveConfig.TimeLimit;

	SetWaveState(EWaveState::InProgress);

	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle,
		this,
		&AWaveGameMode::TickCountdown,
		1.0f,
		true
	);
}

void AWaveGameMode::TickCountdown()
{
	RemainingTime -= 1.0f;
	OnTimerUpdated.Broadcast(RemainingTime);

	if (RemainingTime <= 0.0f)
	{
		EndWave(false);
	}
}

void AWaveGameMode::OnCoinCollected()
{
	if (WaveState != EWaveState::InProgress) return;

	++CollectedCoins;
	OnCoinCountChanged.Broadcast(CollectedCoins, WaveConfig.CoinTarget);
}

void AWaveGameMode::OnEnemyKilled()
{
	if (WaveState != EWaveState::InProgress) return;

	++KilledEnemies;
	OnKillCountChanged.Broadcast(KilledEnemies, WaveConfig.KillTarget);
}

void AWaveGameMode::OnPlayerReachedGoal()
{
	if (WaveState != EWaveState::InProgress || bGoalReached) return;

	// 코인 목표가 있으면 체크
	if (WaveConfig.CoinTarget > 0 && CollectedCoins < WaveConfig.CoinTarget) return;

	// 킬 목표가 있으면 체크
	if (WaveConfig.KillTarget > 0 && KilledEnemies < WaveConfig.KillTarget) return;

	bGoalReached = true;
	EndWave(true);
}

void AWaveGameMode::OnPlayerDied()
{
	if (WaveState != EWaveState::InProgress) return;
	EndWave(false);
}

void AWaveGameMode::SetWaveState(EWaveState NewState)
{
	WaveState = NewState;
	OnWaveStateChanged.Broadcast(NewState);
}

void AWaveGameMode::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}

void AWaveGameMode::EndWave(bool bSuccess)
{
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
	SetWaveState(bSuccess ? EWaveState::WaveComplete : EWaveState::WaveFailed);

	if (bSuccess && !WaveConfig.NextLevelName.IsNone())
	{
		FTimerHandle DelayHandle;
		FName NextLevel = WaveConfig.NextLevelName;
		GetWorldTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([this, NextLevel]()
		{
			UGameplayStatics::OpenLevel(this, NextLevel);
		}), 2.0f, false);
	}
}