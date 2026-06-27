// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemSpawner.generated.h"

UCLASS()
class CHAPTER3_3_API AItemSpawner : public AActor
{
	GENERATED_BODY()
public:
	AItemSpawner();

	// 에디터에서 DataTable 연결 (FItemDataTableRow 타입)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	UDataTable* ItemDataTable;

	// BeginPlay 자동 스폰 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool bSpawnOnBeginPlay = true;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnItems();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void ClearItems();

protected:
	virtual void BeginPlay() override;

	// 스폰 영역
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	class UBoxComponent* SpawnBounds;

	UPROPERTY()
	TArray<AActor*> SpawnedItems;

	FVector GetRandomPointInBounds() const;
};