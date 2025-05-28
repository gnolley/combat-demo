// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/ACombatCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AACombatCharacter::AACombatCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	auto root = GetRootComponent();

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->SetupAttachment(root);
	CameraArm->TargetArmLength = 300;
	CameraArm->bEnableCameraLag = true;
	CameraArm->CameraLagSpeed = 5.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AACombatCharacter::BeginPlay()
{
	Super::BeginPlay();
	MovementComponent = Cast<UCharacterMovementComponent>(GetCharacterMovement());
	SetGait(ECharacterGait::Walk);
}

void AACombatCharacter::SetGaitSettings(const FGaitSettings& Settings)
{
	auto* movementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	check(IsValid(movementComponent));

	movementComponent->MaxWalkSpeed = Settings.MaxSpeed;
	movementComponent->MaxAcceleration = Settings.MaxAcceleration;
	movementComponent->BrakingDecelerationWalking = Settings.BrakeSpeed;
	movementComponent->RotationRate = {0.f, Settings.TurnSpeed, 0.f};
}

void AACombatCharacter::SetGait(const ECharacterGait& InGait)
{
	Gait = InGait;

	auto settings = FindGaitSettings(InGait);
	if (settings.IsSet())
	{
		SetGaitSettings(settings.GetValue());
	}
}

FGaitSettings AACombatCharacter::GetCurrentGaitSettings()
{
	auto settings = FindGaitSettings(Gait);
	if (settings.IsSet())
	{
		return settings.GetValue();
	}
	
	return GaitSettings.IsEmpty() ? FGaitSettings() : GaitSettings.begin().Value();
}

TOptional<const FGaitSettings> AACombatCharacter::FindGaitSettings(ECharacterGait InGait)
{
	return GaitSettings.Contains(InGait) ? GaitSettings[InGait] : TOptional<const FGaitSettings>();
}

// Called every frame
void AACombatCharacter::Tick(float DeltaTime)
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

// Called to bind functionality to input
void AACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

