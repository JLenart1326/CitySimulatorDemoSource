#include "Core/Components/CDUserInterfaceManager.h"
#include "Core/CitySimulatorDemoCharacter.h"
#include "UserInterface/CDMainHUD.h"
#include "CitySimulatorDemoFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "Core/CitySimulatorDemo.h"

// Sets default values for this component's properties
UCDUserInterfaceManager::UCDUserInterfaceManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCDUserInterfaceManager::ComponentInit_Implementation(ACitySimulatorDemoCharacter* Owner)
{
	if (Owner == nullptr)
	{
		UE_LOG(LogCitySimulatorDemo, Error, TEXT("UCDUserInterfaceManager ComponentInit Failed - Owner is not valid."));
		return;
	}
	OwnerCharacter = Owner;
	UE_LOG(LogCitySimulatorDemo, Log, TEXT("UCDUserInterfaceManager ComponentInit finished. OwnerCharacter: %s"), *(OwnerCharacter->GetName()));
	PrepareMainHUD();
}


void UCDUserInterfaceManager::PlayFadeSequence()
{
	SetPlayerInputNewEnabled(false);

	if (MainHUD == nullptr)
	{
		UE_LOG(LogCitySimulatorDemo, Error, TEXT("UCDUserInterfaceManager MainHUD is invalid"));
		return;
	}

	MainHUD->OnFadeInAnimationFinished.AddDynamic(this, &UCDUserInterfaceManager::ProcessOnFadeInAnimationFinished);
	MainHUD->OnFadeOutAnimationFinished.AddDynamic(this, &UCDUserInterfaceManager::ProcessOnFadeOutAnimationFinished);
	MainHUD->PlayFadeInAnimation();
}

void UCDUserInterfaceManager::PrepareMainHUD()
{
	if (MainHUDClass == nullptr) {
		UE_LOG(LogCitySimulatorDemo, Error, TEXT("UCDUserInterfaceManager MainHUDClass to create is invalid"));
		return;
	}
	MainHUD = Cast<UCDMainHUD>(CreateWidget<UUserWidget>(GetWorld(), MainHUDClass));

	if (MainHUD) 
	{
		MainHUD->bCoveredOnStartup = bHUDCoveredOnStartup;
		MainHUD->AddToViewport();
		
		if (!bSkipIntroAnimation) {
			StartupFadeIntro();
		}
	}
	else {
		UE_LOG(LogCitySimulatorDemo, Error, TEXT("UCDUserInterfaceManager MainHUD widget creation failed."));
	}

}

void UCDUserInterfaceManager::StartupFadeIntro()
{
	SetPlayerInputNewEnabled(false);

	if (MainHUD == nullptr) 
	{
		UE_LOG(LogCitySimulatorDemo, Error, TEXT("UCDUserInterfaceManager MainHUD is invalid"));
		return;
	}

	MainHUD->OnFadeOutAnimationFinished.AddDynamic(this, &UCDUserInterfaceManager::ProcessOnFadeOutAnimationFinished);
	MainHUD->PlayFadeOutAnimation();
}

void UCDUserInterfaceManager::SetPlayerInputNewEnabled(bool NewEnabled)
{
	if (OwnerCharacter == nullptr) {
		UE_LOG(LogCitySimulatorDemo, Error, TEXT("UCDUserInterfaceManager OwnerCharacter is invalid"));
		return;
	}

	OwnerCharacter->SetNewInputEnabled(NewEnabled);
}

void UCDUserInterfaceManager::ProcessOnFadeOutAnimationFinished()
{
	if (MainHUD == nullptr)
	{
		UE_LOG(LogCitySimulatorDemo, Error, TEXT("UCDUserInterfaceManager MainHUD is invalid"));
		return;
	}

	MainHUD->OnFadeOutAnimationFinished.RemoveDynamic(this, &UCDUserInterfaceManager::ProcessOnFadeOutAnimationFinished);

	SetPlayerInputNewEnabled(true);
}

void UCDUserInterfaceManager::ProcessOnFadeInAnimationFinished()
{
	if (MainHUD == nullptr)
	{
		UE_LOG(LogCitySimulatorDemo, Error, TEXT("UCDUserInterfaceManager MainHUD is invalid"));
		return;
	}

	MainHUD->OnFadeInAnimationFinished.RemoveDynamic(this, &UCDUserInterfaceManager::ProcessOnFadeInAnimationFinished);

	OnScreenFullyCovered.Broadcast(this);
}

// Called when the game starts
void UCDUserInterfaceManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCDUserInterfaceManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

