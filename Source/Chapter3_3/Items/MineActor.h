// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MineActor.generated.h"

UCLASS()
class CHAPTER3_3_API AMineActor : public AActor
{
	GENERATED_BODY()
public:
	AMineActor();

	//지뢰별 데미지 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float Damage = 50.0f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mine")
	class USphereComponent* TriggerSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mine")
	class UStaticMeshComponent* MeshComp;

	bool bExploded = false;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};