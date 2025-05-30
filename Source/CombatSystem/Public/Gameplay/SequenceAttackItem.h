#pragma once

#include "SequenceAttackItem.generated.h"

USTRUCT(BlueprintType)
struct FSequenceAttackItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	TObjectPtr<UAnimMontage> Montage {};
};
