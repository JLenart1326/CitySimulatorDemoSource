#include "Core/Components/CDInteractionManager.h"
#include "Core/CitySimulatorDemoCharacter.h"
#include <Camera/CameraComponent.h>
#include "Core/Interfaces/CDInteractableInterface.h"
#include "Core/CitySimulatorDemo.h"

// Sets default values for this component's properties
UCDInteractionManager::UCDInteractionManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCDInteractionManager::ComponentInit_Implementation(ACitySimulatorDemoCharacter* Owner)
{
	if (Owner == nullptr)
	{
		UE_LOG(LogCitySimulatorDemo, Log, TEXT("UCDUserInterfaceManager ComponentInit Failed - OwnerCharacter is not valid."));
		return;
	}
	OwnerCharacter = Owner;
	UE_LOG(LogCitySimulatorDemo, Log, TEXT("UCDUserInterfaceManager ComponentInit finished. OwnerCharacter: %s"), *(OwnerCharacter->GetName()));
}

void UCDInteractionManager::RequestStartInteraction()
{
	UE_LOG(LogCitySimulatorDemo, Log, TEXT("Interaction Started."));

	if (!OwnerCharacter) {
		return;
	}

	FHitResult Hit;
	FVector Start = OwnerCharacter->GetFollowCamera()->GetComponentLocation();
	FVector End = Start + (OwnerCharacter->GetFollowCamera()->GetForwardVector() * InteractionRange);
	//FVector End = Start + (OwnerCharacter->GetActorForwardVector()*Range);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);
	FCollisionResponseParams ResponseParams;
	FColor DebugResultLineColor = FColor::Red;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params,
		ResponseParams
	);

	if (bHit) {
		InteractingActor = Hit.GetActor();
		if (InteractingActor.IsValid())
		{
			if (InteractingActor->Implements<UCDInteractableInterface>()) {
				DebugResultLineColor = FColor::Green;

				ICDInteractableInterface::Execute_OnInteractionStarted(InteractingActor.Get(), OwnerCharacter);
			}
			else {
				InteractingActor = nullptr;
			}
		}
	}
	DrawDebugSphere(
		GetWorld(),
		Hit.Location,
		10.f,
		10.f,
		DebugResultLineColor,
		false,
		5.f,
		(uint8)0U,
		1.f
	);
}

void UCDInteractionManager::RequestStopInteraction()
{
	UE_LOG(LogCitySimulatorDemo, Log, TEXT("Interaction Stopped."));

	if (InteractingActor.IsValid()) {
		bool bShouldStopInteraction = ICDInteractableInterface::Execute_IsHoldable(InteractingActor.Get());
		if (bShouldStopInteraction) {
			ICDInteractableInterface::Execute_OnInteractionStopped(InteractingActor.Get(), OwnerCharacter);
		}
		InteractingActor = nullptr;
	}
}


// Called when the game starts
void UCDInteractionManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCDInteractionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

