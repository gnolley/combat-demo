// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystemPlayerController.h"

#include "Characters/CombatCharacter.h"
#include "Engine/OverlapResult.h"
#include "Gameplay/TargetableInterface.h"

void ACombatSystemPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateTargeting();
}

struct TargetOverlap
{
	double dot;
	AActor* target;
	FVector location;

	bool valid = true;

	static TargetOverlap Invalid()
	{
		return {
			.dot = 999.f,
			.valid = false,
		};
	}
};

void ACombatSystemPlayerController::UpdateTargeting()
{
	// Don't bother searching for new target, we're already locked
	if (HasTarget()) return;
	
	const auto player = GetPawn();
	const auto playerLoc = player->GetActorLocation();
	const auto cameraLoc = PlayerCameraManager->GetCameraLocation();
	const auto cameraForward = PlayerCameraManager->GetActorForwardVector();

	const float maxAngleCos = FMath::Cos(FMath::DegreesToRadians(TargetingParams.MaxTargetAngle));

	// TODO: Prealloc for less heap
	TArray<FOverlapResult> Overlaps {};
	
	// Overlap for actors
	const auto shape = FCollisionShape::MakeSphere(TargetingParams.MaxTargetDistance);	
	if (GetWorld()->OverlapMultiByChannel(Overlaps, playerLoc, FQuat::Identity, ECollisionChannel::ECC_WorldDynamic, shape) == false)
	{
		HighlightedActor = nullptr;
		return;
	}
	
	TArray<TargetOverlap> targets {};
	targets.SetNum(Overlaps.Num());
	
	for(int i=0; i < Overlaps.Num(); ++i)
	{
		auto actor = Overlaps[i].GetActor();
		if (IsValid(actor) == false || actor == player)
		{
			targets[i] = TargetOverlap::Invalid(); // we can only target actors
			continue;
		}
		
		auto targetable = Cast<ITargetable>(actor);
		if (!targetable)
		{
			targets[i] = TargetOverlap::Invalid();
			continue;
		}
		
		auto loc = actor->GetActorLocation();
		auto disp = loc - cameraLoc;
		disp.Normalize();

		float dot = FVector::DotProduct(cameraForward, disp);
		if (dot < maxAngleCos)
		{
			targets[i] = TargetOverlap::Invalid();
			continue;
		}
		
		targets[i] = {
			.dot = dot,
			.target = actor,
			.location = loc	
		};
	}

	if (targets.Num() <= 0)
	{
		HighlightedActor = nullptr;
		return;
	}
	
	// Filter by camera view
	targets.Sort([](const TargetOverlap& lhs, const TargetOverlap& rhs)
	{
		if (lhs.valid != rhs.valid)
		{
			return lhs.valid;
		}
		
		return lhs.dot > rhs.dot;
	});

	// Select best target

	if (targets[0].valid == false)
	{
		HighlightedActor = nullptr;
		return;
	}
	
	HighlightedActor = targets[0].target;
}

void ACombatSystemPlayerController::SetTarget(AActor* NewTarget)
{
	if (NewTarget == Target) return;

	Target = NewTarget;
	
	auto* PlayerPawn = Cast<ACombatCharacter>(GetPawn());
	if (IsValid(Player))
	{
		PlayerPawn->SetTarget(NewTarget);
	}
	
	if (NewTarget)
	{
		UE_LOG(LogTemp, Display, TEXT("Target Set: %s"), *NewTarget->GetName());
	}
	else UE_LOG(LogTemp, Display, TEXT("Target Unset"));
}

void ACombatSystemPlayerController::UnlockTarget()
{
	SetTarget(nullptr);
}
