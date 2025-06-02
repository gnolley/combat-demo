// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystemPlayerController.h"

#include "Engine/OverlapResult.h"

void ACombatSystemPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateTargeting();
}

struct TargetOverlap
{
	double dot;
	FVector location;
};

void ACombatSystemPlayerController::UpdateTargeting()
{
	const auto player = GetPawn();
	const auto playerLoc = player->GetActorLocation();
	const auto cameraLoc = PlayerCameraManager->GetCameraLocation();
	const auto cameraForward = PlayerCameraManager->GetActorForwardVector();

	// TODO: Prealloc for less heap
	TArray<FOverlapResult> Overlaps {};
	
	// Overlap for actors
	const auto shape = FCollisionShape::MakeSphere(TargetingParams.MaxTargetDistance);
	GetWorld()->OverlapMultiByChannel(Overlaps, playerLoc, FQuat::Identity, ECollisionChannel::ECC_WorldDynamic, shape);
	
	std::vector<TargetOverlap> targets {static_cast<size_t>(Overlaps.Num())};
	for(int i=0; i < Overlaps.Num(); ++i)
	{
		auto loc = Overlaps[i].GetActor()->GetActorLocation();
		auto disp = loc - cameraLoc;
		disp.Normalize();
		
		targets[i] = {
			.dot = FVector::DotProduct(cameraForward, disp),
			.location = loc	
		};
	}
	
	// Filter by camera view
	Overlaps.Sort([](FOverlapResult* lhs, FOverlapResult* rhs)
	{
		
	});

	// Select best target
	
}
