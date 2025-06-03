#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	auto root = GetRootComponent();

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->SetupAttachment(root);
	CameraArm->TargetArmLength = 300;
	CameraArm->bEnableCameraLag = true;
	CameraArm->CameraLagSpeed = 5.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UnlockedSettings.CameraArmLength = CameraArm->TargetArmLength;
	UnlockedSettings.CameraArmSocketOffset = CameraArm->SocketOffset;
	UnlockedSettings.CameraLag = CameraArm->CameraLagSpeed;
}

void APlayerCharacter::SetTarget(AActor* NewTarget)
{
	Super::SetTarget(NewTarget);

	auto& settings = NewTarget ? LockedOnSettings : UnlockedSettings;
	
	CameraArm->TargetArmLength = settings.CameraArmLength;
	CameraArm->SocketOffset = settings.CameraArmSocketOffset;
	CameraArm->CameraLagSpeed = settings.CameraLag;
}
