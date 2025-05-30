// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "Weapon.generated.h"

UENUM()
enum class EWeaponHand : uint8
{
	MainHand = 0b01,
	OffHand = 0b10,
	Both = 0b11,
};

UCLASS()
class COMBATSYSTEM_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	
	void Equip(TObjectPtr<UAbilitySystemComponent> AbilitySystem);
	void UnEquip();

	EWeaponHand GetHandedness() const { return Handedness; };
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateAbilitySpec(TArray<FGameplayAbilitySpec>& Specs, const TSubclassOf<UGameplayAbility>& Ability);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equiping")
	EWeaponHand Handedness;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TSubclassOf<UGameplayAbility> PrimaryAbility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TSubclassOf<UGameplayAbility> SecondaryAbility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TSubclassOf<UGameplayAbility> SpecialAbility;

	TArray<FGameplayAbilitySpec> AbilitySpecs {};
	TObjectPtr<UAbilitySystemComponent> BoundSystem;
	TArray<FGameplayAbilitySpecHandle> BoundAbilities {};
};
