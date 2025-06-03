// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemInterface.h"
#include "Gameplay/TargetableInterface.h"
#include "Gameplay/Weapon.h"
#include "Gameplay/Attributes/DamageableAttributes.h"
#include "CombatCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterGait : uint8
{
	Walk,
	Run,
};

USTRUCT(BlueprintType)
struct FGaitSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxSpeed { 200 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxAcceleration { 150 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> AccelerationCurve {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BrakeSpeed { 120 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TurnSpeed { 220 };
};

UCLASS()
class COMBATSYSTEM_API ACombatCharacter : public ACharacter,
	public IAbilitySystemInterface, public ITargetable
{
	GENERATED_BODY()

public:
	ACombatCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category="Movement")
	void SetGaitSettings(const FGaitSettings& Settings);

	UFUNCTION(BlueprintCallable, Category="Movement")
	FGaitSettings GetCurrentGaitSettings();
	
	TOptional<const FGaitSettings> FindGaitSettings(ECharacterGait InGait);

	TObjectPtr<AWeapon> GetEquippedWeapon(EWeaponHand Hand);
	
public:	
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; }

	UFUNCTION(BlueprintCallable, Category="Abilities")
	FGameplayAbilitySpecHandle GetPrimaryAbility() const;
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	void SetGait(const ECharacterGait& InGait);

	
	UFUNCTION(BlueprintCallable, Category="Weapons")
	void EquipWeapon(AWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category="Weapons")
	void UnEquip(EWeaponHand Hand);
	
	UFUNCTION(BlueprintCallable, Category="Weapons")
	TArray<AWeapon*> GetWeapons();

	
	UFUNCTION(BlueprintCallable, Category="Combat")
	virtual void SetTarget(AActor* NewTarget) {  Target = NewTarget; }

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool HasTarget() const {  return IsValid(Target); }
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystem;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Attibutes")
	TObjectPtr<UDamageableAttributes> DamageableAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	ECharacterGait Gait { ECharacterGait::Walk };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	TMap<ECharacterGait, FGaitSettings> GaitSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	TObjectPtr<UCurveFloat> BrakingCurve {};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	TObjectPtr<AActor> Target {};
	
	TObjectPtr<AWeapon> MainHand {};
	TObjectPtr<AWeapon> OffHand {};
	
private:
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
};
