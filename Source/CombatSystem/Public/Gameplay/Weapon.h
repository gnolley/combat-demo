// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FWeaponHitInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> HitActor;

	UPROPERTY(BlueprintReadOnly)
	FVector ImpulseNormal;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponHit, FWeaponHitInfo, HitInfo);

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

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void ActivateWeapon();
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void DeactivateWeapon();
	UFUNCTION(BlueprintCallable, Category="Weapon")
	inline bool IsWeaponActive() const {return WeaponActive; }

	EWeaponHand GetHandedness() const { return Handedness; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void TryAffectActor(AActor* OtherActor);
	
	void CreateAbilitySpec(TArray<FGameplayAbilitySpec>& Specs, const TSubclassOf<UGameplayAbility>& Ability);
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponHit OnWeaponHit;
	
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

	bool WeaponActive;
};
