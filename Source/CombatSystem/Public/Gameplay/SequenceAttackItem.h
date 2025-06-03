#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "SequenceAttackItem.generated.h"

USTRUCT(BlueprintType)
struct FSequenceAttackItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	TObjectPtr<UAnimMontage> Montage {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
	TSubclassOf<UGameplayEffect> AttackEffect {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
	bool AllowOffhandHit;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dash")
	float DashLength = 150;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dash")
	float DashDuration = 0.2f;
};
