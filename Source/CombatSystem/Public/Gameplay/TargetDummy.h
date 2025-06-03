// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "TargetableInterface.h"
#include "Gameplay/Attributes/DamageableAttributes.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "TargetDummy.generated.h"

UCLASS()
class COMBATSYSTEM_API ATargetDummy : public ACharacter,
	public IAbilitySystemInterface, public ITargetable
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATargetDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	void SetupAttributes();
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attibutes")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;
	
	UPROPERTY()
	TObjectPtr<UDamageableAttributes> DamageableAttributes;
};
