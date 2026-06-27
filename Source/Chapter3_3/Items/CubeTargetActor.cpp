// Fill out your copyright notice in the Description page of Project Settings.
#include "CubeTargetActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

ACubeTargetActor::ACubeTargetActor()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	CollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = CollisionBox;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(CubeMesh.Object);
	}
}

float ACubeTargetActor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (bDestroyed) return 0.f;
	bDestroyed = true;
	Destroy();
	return DamageAmount;
}

void ACubeTargetActor::Destroyed()
{
	Super::Destroyed();

	// Enemy_Tank와 동일한 경로 — OnEnemyKilled로 킬 카운트 증가
	if (AGameModeBase* GM = UGameplayStatics::GetGameMode(this))
	{
		if (UFunction* Func = GM->FindFunction(TEXT("OnEnemyKilled")))
		{
			GM->ProcessEvent(Func, nullptr);
		}
	}
}