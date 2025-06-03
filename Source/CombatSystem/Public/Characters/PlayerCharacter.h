#pragma once
#include "CombatCharacter.h"
#include "PlayerCharacter.generated.h"

USTRUCT(BlueprintType)
struct FLockOnSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	float CameraArmLength {250.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	float CameraLag {4.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FVector CameraArmSocketOffset {0.f, 0.f, 0.f};
};

UCLASS()
class APlayerCharacter : public ACombatCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetTarget(AActor* NewTarget) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Camera")
	TObjectPtr<USpringArmComponent> CameraArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Targeting")
	FLockOnSettings LockedOnSettings {};

	// Set at runtime
	FLockOnSettings UnlockedSettings {};
};
