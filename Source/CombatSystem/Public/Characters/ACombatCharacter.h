// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "ACombatCharacter.generated.h"

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
	float Acceleration { 150 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BrakeSpeed { 120 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TurnSpeed { 220 };
};

UCLASS()
class COMBATSYSTEM_API AACombatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AACombatCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Movement")
	void SetGaitSettings(const FGaitSettings& Settings);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<USpringArmComponent> CameraArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	ECharacterGait Gait { ECharacterGait::Walk };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	TMap<ECharacterGait, FGaitSettings> GaitSettings;
};
