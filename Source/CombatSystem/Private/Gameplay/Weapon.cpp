// Fill out your copyright notice in the Description page of Project Settings.
#include "Gameplay/Weapon.h"
#include "AbilitySystemComponent.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(RootComponent);	
}

void AWeapon::CreateAbilitySpec(TArray<FGameplayAbilitySpec>& Specs, const TSubclassOf<UGameplayAbility>& Ability)
{
	if (IsValid(Ability) == false) return;

	FGameplayAbilitySpec Spec {Ability, 0};
	Spec.SourceObject = this;
	
	Specs.Add(Spec);	
}

void AWeapon::Equip(TObjectPtr<UAbilitySystemComponent> AbilitySystem)
{
	BoundSystem = AbilitySystem;
	
	for (auto Ability : AbilitySpecs)
	{
		BoundAbilities.Add(BoundSystem->GiveAbility(Ability));
	}
}

void AWeapon::UnEquip()
{
	if (IsValid(BoundSystem) == false) return;
	DetachFromActor({EDetachmentRule::KeepWorld, true});

	for (auto Ability : BoundAbilities)
	{
		BoundSystem->ClearAbility(Ability);
	}

	BoundAbilities.Empty();
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Guard against race condition
	CreateAbilitySpec(AbilitySpecs, PrimaryAbility);
    CreateAbilitySpec(AbilitySpecs, SecondaryAbility);
    CreateAbilitySpec(AbilitySpecs, SecondaryAbility);
}

