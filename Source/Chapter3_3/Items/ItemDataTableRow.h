// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataTableRow.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Coin        UMETA(DisplayName = "Coin"),
	HealthPack  UMETA(DisplayName = "Health Pack"),
	AmmoPack    UMETA(DisplayName = "Ammo Pack"),
	CubeTarget  UMETA(DisplayName = "Cube Target"),
};

// DataTable 에디터에서 아이템 종류별로 행을 추가해 관리
USTRUCT(BlueprintType)
struct CHAPTER3_3_API FItemDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType = EItemType::Coin;

	// 스폰할 액터 클래스 (BP_CoinActor, BP_HealthPack 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AActor> ItemClass;

	// 이 행의 아이템을 몇 개 스폰할지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (ClampMin = "0"))
	int32 SpawnCount = 5;

	// 랜덤 스폰 시 가중치 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (ClampMin = "0.0"))
	float SpawnWeight = 1.0f;
};