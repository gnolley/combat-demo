// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/TargetDummy.h"

// Sets default values
ATargetDummy::ATargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	DamageableAttributes = CreateDefaultSubobject<UDamageableAttributes>(TEXT("DamageableAttributes"));
}

// Called when the game starts or when spawned
void ATargetDummy::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DamageableAttributes->GetHealthAttribute())
		.AddLambda([](const FOnAttributeChangeData& data)
		{
			UE_LOG(LogTemp, Log, TEXT("Dummy Health Changed: %f"), data.NewValue)
		});
	SetupAttributes();

	DamageableAttributes->SetHealth(100.f);
}

// Called every frame
void ATargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATargetDummy::SetupAttributes()
{
	if (IsValid(AbilitySystemComponent) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Target Dummy Ability System Invalid."));
		return;
	}

	if (IsValid(DefaultAttributes) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Target Dummy Missing Default Attributes."));
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle DefaultAttributesSpec = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 0, EffectContext);
	if (DefaultAttributesSpec.IsValid())
	{
		auto _ = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DefaultAttributesSpec.Data.Get());
	}
}

