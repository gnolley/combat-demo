// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CombatSystemPlayerController.generated.h"

USTRUCT(BlueprintType)
struct FTargetingParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Targeting")
	float MaxTargetAngle {60.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Targeting")
	float MaxTargetDistance {2000};
};

/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API ACombatSystemPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void UpdateTargeting();

	UFUNCTION(BlueprintCallable, Category="CombatSystemPlayerController|Targeting")
	AActor* GetTarget() const { return Target;}

	UFUNCTION(BlueprintCallable, Category="CombatSystemPlayerController|Targeting")
	bool HasTarget() const {return IsValid(Target); }	

protected:
	UFUNCTION(BlueprintCallable, Category="CombatSystemPlayerController|Targeting")
	void SetTarget(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category="CombatSystemPlayerController|Targeting")
	void UnlockTarget();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Targeting")
	FTargetingParams TargetingParams {};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Targeting")
	TObjectPtr<AActor> HighlightedActor {};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Targeting")
	TObjectPtr<AActor> Target {};
};
