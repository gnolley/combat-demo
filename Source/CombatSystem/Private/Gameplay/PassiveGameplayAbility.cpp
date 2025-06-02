// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PassiveGameplayAbility.h"
#include "AbilitySystemComponent.h"

void UPassiveGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
}
