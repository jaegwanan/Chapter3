#include "ItemSpawner.h"
#include "Components/BoxComponent.h"
#include "ItemDataTableRow.h"

AItemSpawner::AItemSpawner()
{
	SpawnBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBounds"));
	SpawnBounds->SetBoxExtent(FVector(500.f, 500.f, 50.f));
	SpawnBounds->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnBounds->ShapeColor = FColor::Green;
	SpawnBounds->SetVisibility(true);
	RootComponent = SpawnBounds;
}

void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (bSpawnOnBeginPlay)
	{
		SpawnItems();
	}
}

void AItemSpawner::SpawnItems()
{
	if (!ItemDataTable) return;

	TArray<FItemDataTableRow*> Rows;
	ItemDataTable->GetAllRows<FItemDataTableRow>(TEXT("ItemSpawner"), Rows);

	for (const FItemDataTableRow* Row : Rows)
	{
		if (!Row || !Row->ItemClass) continue;

		for (int32 i = 0; i < Row->SpawnCount; ++i)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AActor* Spawned = GetWorld()->SpawnActor<AActor>(
				Row->ItemClass,
				GetRandomPointInBounds(),
				FRotator::ZeroRotator,
				Params
			);

			if (Spawned)
			{
				SpawnedItems.Add(Spawned);
			}
		}
	}
}

void AItemSpawner::ClearItems()
{
	for (AActor* Item : SpawnedItems)
	{
		if (IsValid(Item))
		{
			Item->Destroy();
		}
	}
	SpawnedItems.Empty();
}

FVector AItemSpawner::GetRandomPointInBounds() const
{
	const FVector Origin = SpawnBounds->GetComponentLocation();
	const FVector Extent = SpawnBounds->GetScaledBoxExtent();

	return FVector(
		FMath::RandRange(Origin.X - Extent.X, Origin.X + Extent.X),
		FMath::RandRange(Origin.Y - Extent.Y, Origin.Y + Extent.Y),
		Origin.Z + Extent.Z
	);
}