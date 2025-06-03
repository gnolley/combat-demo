// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/CombatCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

const FName MainHandSocket = FName("weapon_socket_r");
const FName OffHandSocket = FName("weapon_socket_l");

// Sets default values
ACombatCharacter::ACombatCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	auto root = GetRootComponent();

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability System Component"));
	DamageableAttributes = CreateDefaultSubobject<UDamageableAttributes>(TEXT("DamageableAttributes"));
}

// Called when the game starts or when spawned
void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();
	MovementComponent = Cast<UCharacterMovementComponent>(GetCharacterMovement());
	SetGait(ECharacterGait::Walk);
}

void ACombatCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(AbilitySystem))
	{
		AbilitySystem->InitAbilityActorInfo(this, this);
	}
}

void ACombatCharacter::SetGaitSettings(const FGaitSettings& Settings)
{
	auto* movementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	check(IsValid(movementComponent));

	movementComponent->MaxWalkSpeed = Settings.MaxSpeed;
	movementComponent->MaxAcceleration = Settings.MaxAcceleration;
	movementComponent->BrakingDecelerationWalking = Settings.BrakeSpeed;
	movementComponent->RotationRate = {0.f, Settings.TurnSpeed, 0.f};
}

FGameplayAbilitySpecHandle ACombatCharacter::GetPrimaryAbility() const
{
	auto abilities = AbilitySystem->GetActivatableAbilities();
	const auto primaryTag = FGameplayTag::RequestGameplayTag(FName("Ability.Primary"));
	
	for (const auto& abilitySpec : abilities)
	{
		auto tags = abilitySpec.Ability->AbilityTags;
		if(tags.HasTag(primaryTag))
		{
			return abilitySpec.Handle;
		}   
	}

	return {};
}

void ACombatCharacter::SetGait(const ECharacterGait& InGait)
{
	Gait = InGait;

	auto settings = FindGaitSettings(InGait);
	if (settings.IsSet())
	{
		SetGaitSettings(settings.GetValue());
	}
}

TObjectPtr<AWeapon> ACombatCharacter::GetEquippedWeapon(EWeaponHand Hand)
{
	switch (Hand)
	{
	default:
	case EWeaponHand::MainHand:
		return MainHand;
	case EWeaponHand::OffHand:
		return OffHand;
	}
}

const FName& GetWeaponSocket(EWeaponHand WeaponHand)
{
	switch (WeaponHand)
	{
	default:
	case EWeaponHand::MainHand:
		return MainHandSocket;
	case EWeaponHand::OffHand:
		return OffHandSocket;
	}
}

void ACombatCharacter::EquipWeapon(AWeapon* Weapon)
{
	if (IsValid(AbilitySystem) == false) return;

	UnEquip(Weapon->GetHandedness());
	
	Weapon->AttachToComponent(GetMesh(), {EAttachmentRule::SnapToTarget, true},
		GetWeaponSocket(Weapon->GetHandedness()));
	
	Weapon->Equip(AbilitySystem);
}

void ACombatCharacter::UnEquip(EWeaponHand Hand)
{
	if (IsValid(AbilitySystem) == false) return;

	
}

FGaitSettings ACombatCharacter::GetCurrentGaitSettings()
{
	auto settings = FindGaitSettings(Gait);
	if (settings.IsSet())
	{
		return settings.GetValue();
	}
	
	return GaitSettings.IsEmpty() ? FGaitSettings() : GaitSettings.begin().Value();
}

TOptional<const FGaitSettings> ACombatCharacter::FindGaitSettings(ECharacterGait InGait)
{
	return GaitSettings.Contains(InGait) ? GaitSettings[InGait] : TOptional<const FGaitSettings>();
}

// Called every frame
void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsValid(MovementComponent) == false) return;

	auto speed = GetVelocity().Length();
	
	if (IsValid(BrakingCurve))
	{
		MovementComponent->BrakingDecelerationWalking = BrakingCurve->GetFloatValue(speed);
	}
	
	auto settings = GetCurrentGaitSettings();
	if (IsValid(settings.AccelerationCurve) == false) return;
	
	auto accel = settings.MaxAcceleration * settings.AccelerationCurve->GetFloatValue(speed / settings.MaxSpeed);
	
	MovementComponent->MaxAcceleration = accel;
}

