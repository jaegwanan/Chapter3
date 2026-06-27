// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Components/ArrowComponent.h"
#include "MyGASPawn.generated.h"

class UAttributeSet;
struct FInputActionValue;

UCLASS()
class CHAPTER3_3_API AMyGASPawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMyGASPawn();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* AbilitySystemComponent;

public:
	// 뷰포트에서 드래그해 총구 위치 지정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UArrowComponent* MuzzlePoint;

protected:

	// 각 서브클래스가 BeginPlay에서 AddAttributeSetSubobject로 등록한 AttributeSet 보관
	UPROPERTY()
	TArray<UAttributeSet*> AttributeSets;

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 사격 입력 → ASC에서 ShootAbilityClass 실행
	UFUNCTION()
	void OnShoot(const FInputActionValue& Value);

	// 각 캐릭터 생성자에서 자신의 어빌리티 클래스를 지정
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayAbility> ShootAbilityClass;

	// 데미지 적용
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	// 서브클래스에서 원하는 AttributeSet을 ASC에 등록할 때 사용
	template<typename T>
	T* AddAttributeSet()
	{
		T* NewSet = NewObject<T>(this);
		AbilitySystemComponent->AddAttributeSetSubobject(NewSet);
		AttributeSets.Add(NewSet);
		return NewSet;
	}
};