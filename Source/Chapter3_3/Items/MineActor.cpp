#include "MineActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GAS/MyGASPawn.h"
#include "Kismet/GameplayStatics.h"

AMineActor::AMineActor()
{
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetSphereRadius(80.0f);
	TriggerSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = TriggerSphere;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}

void AMineActor::BeginPlay()
{
	Super::BeginPlay();
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AMineActor::OnOverlapBegin);
}

void AMineActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (bExploded || !Cast<AMyGASPawn>(OtherActor)) return;

	bExploded = true;

	// TakeDamage → AMyGASPawn::TakeDamage → Health 속성 감소 → 사망 시 WaveGameMode 알림
	UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, UDamageType::StaticClass());

	Destroy();
}