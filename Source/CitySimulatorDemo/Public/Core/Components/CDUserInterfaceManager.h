#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Interfaces/CDComponents.h"
#include "CDUserInterfaceManager.generated.h"

class UCDUserInterfaceManager;
class UCDMainHUD;
class ACitySimulatorDemoCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScreenCoverEvent, UCDUserInterfaceManager*, TargetHUDComponent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CITYSIMULATORDEMO_API UCDUserInterfaceManager : public UActorComponent, public ICDComponents
{
	GENERATED_BODY()

protected:
	//Main HUD
	UPROPERTY()
	TObjectPtr<UCDMainHUD> MainHUD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	bool bHUDCoveredOnStartup = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	bool bSkipIntroAnimation = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainHUDClass;

	UPROPERTY()
	//Owner
	TObjectPtr<ACitySimulatorDemoCharacter> OwnerCharacter;

public:
	//Delegates
	UPROPERTY(BlueprintAssignable, Category="UI|Animation")
	FScreenCoverEvent OnScreenFullyCovered;

private:
	//Animation delegates
	UFUNCTION()
	void ProcessOnFadeOutAnimationFinished();

	UFUNCTION()
	void ProcessOnFadeInAnimationFinished();

public:
	//Getters
	UFUNCTION(BlueprintPure, Category = "UI", meta = (CompactNodeTitle = "Get MainHUD"))
	UCDMainHUD* GetMainHUD() const { return MainHUD; }

	UFUNCTION(BlueprintCallable, Category="UI|Animation")
	void PlayFadeSequence();

private:
	void PrepareMainHUD();
	void StartupFadeIntro();
	void SetPlayerInputNewEnabled(bool NewEnabled);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UCDUserInterfaceManager();

	//ICDComponents Interface Implementation
	virtual void ComponentInit_Implementation(ACitySimulatorDemoCharacter* Owner) override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
