// Fill out your copyright notice in the Description page of Project Settings.
#include "KillZone.h"
#include "Components/BoxComponent.h"
#include "GAS/MyGASPawn.h"
#include "Wave/WaveGameMode.h"
#include "Kismet/GameplayStatics.h"

AKillZone::AKillZone()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(10000.f, 10000.f, 100.f));
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TriggerBox->ShapeColor = FColor::Red;
	RootComponent = TriggerBox;
}

void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AKillZone::OnOverlapBegin);
}

void AKillZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<AMyGASPawn>(OtherActor)) return;

	if (AWaveGameMode* GM = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnPlayerDied();
	}
}