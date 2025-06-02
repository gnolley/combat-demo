// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PassiveGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UPassiveGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
