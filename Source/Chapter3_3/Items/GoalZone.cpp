#include "GoalZone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GAS/MyGASPawn.h"
#include "Wave/WaveGameMode.h"
#include "Kismet/GameplayStatics.h"

AGoalZone::AGoalZone()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = TriggerBox;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}

void AGoalZone::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGoalZone::OnOverlapBegin);
}

void AGoalZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<AMyGASPawn>(OtherActor)) return;

	if (AWaveGameMode* GM = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		// 코인 미달성이면 WaveGameMode 내부에서 자동으로 무시됨
		GM->OnPlayerReachedGoal();
	}
}