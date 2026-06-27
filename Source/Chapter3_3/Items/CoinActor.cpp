// Fill out your copyright notice in the Description page of Project Settings.
#include "CoinActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GAS/MyGASPawn.h"
#include "Wave/WaveGameMode.h"
#include "Kismet/GameplayStatics.h"

ACoinActor::ACoinActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(60.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionSphere;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}

void ACoinActor::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACoinActor::OnOverlapBegin);
}

void ACoinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
}

void ACoinActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (bCollected || !Cast<AMyGASPawn>(OtherActor)) return;

	bCollected = true;

	if (AWaveGameMode* GM = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnCoinCollected();
	}

	Destroy();
}