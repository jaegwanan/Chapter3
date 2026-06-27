// Fill out your copyright notice in the Description page of Project Settings.
#include "WaveHUD.h"
#include "WaveHUDWidget.h"
#include "Wave/WaveGameMode.h"
#include "GAS/MyGASPawn.h"
#include "GAS/Attributes/HealthAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

AWaveHUD::AWaveHUD() {}

void AWaveHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!HUDWidgetClass) return;

	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);

	HUDWidget = CreateWidget<UWaveHUDWidget>(PC, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}

	if (AWaveGameMode* GM = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnTimerUpdated.AddDynamic(this, &AWaveHUD::HandleTimerUpdated);
		GM->OnCoinCountChanged.AddDynamic(this, &AWaveHUD::HandleCoinCountChanged);
		GM->OnWaveStateChanged.AddDynamic(this, &AWaveHUD::HandleWaveStateChanged);
		GM->OnKillCountChanged.AddDynamic(this, &AWaveHUD::HandleKillCountChanged);
	}

	// 폰 빙의 완료 후 체력 델리게이트 바인딩
	BindHealthDelegate();
}

void AWaveHUD::BindHealthDelegate()
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	AMyGASPawn* GASPawn = Cast<AMyGASPawn>(PC->GetPawn());
	if (!GASPawn) return;

	UAbilitySystemComponent* ASC = GASPawn->GetAbilitySystemComponent();
	if (!ASC) return;

	BoundASC = ASC;

	// Health 속성 변경될 때마다 OnHealthChanged 호출
	ASC->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetHealthAttribute())
		.AddUObject(this, &AWaveHUD::OnHealthChanged);

	// 초기값 즉시 표시
	if (const UHealthAttributeSet* HealthSet = ASC->GetSet<UHealthAttributeSet>())
	{
		if (HUDWidget)
		{
			HUDWidget->UpdateHealth(HealthSet->GetHealth(), HealthSet->GetMaxHealth());
		}
	}
}

void AWaveHUD::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (!HUDWidget || !BoundASC) return;

	const UHealthAttributeSet* HealthSet = BoundASC->GetSet<UHealthAttributeSet>();
	if (!HealthSet) return;

	HUDWidget->UpdateHealth(Data.NewValue, HealthSet->GetMaxHealth());
}

void AWaveHUD::HandleTimerUpdated(float RemainingSeconds)
{
	if (HUDWidget) HUDWidget->UpdateTimer(RemainingSeconds);
}

void AWaveHUD::HandleCoinCountChanged(int32 Collected, int32 Target)
{
	if (HUDWidget) HUDWidget->UpdateCoins(Collected, Target);
}

void AWaveHUD::HandleKillCountChanged(int32 Killed, int32 Target)
{
	if (HUDWidget) HUDWidget->UpdateKillCount(Killed, Target);
}

void AWaveHUD::HandleWaveStateChanged(EWaveState NewState)
{
	if (NewState == EWaveState::WaveComplete || NewState == EWaveState::WaveFailed)
	{
		if (HUDWidget) HUDWidget->ShowWaveResult(NewState);

		// 결과 화면에서 마우스 커서 활성화 → 재시작 버튼 클릭 가능
		if (APlayerController* PC = GetOwningPlayerController())
		{
			FInputModeUIOnly UIMode;
			UIMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(UIMode);
			PC->SetShowMouseCursor(true);
		}
	}
}