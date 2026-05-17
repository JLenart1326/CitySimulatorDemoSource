#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CitySimulatorDemoCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCDInteractionManager;
class UCDUserInterfaceManager;
class UInputAction;
class ACitySimulatorDemoPlayerController;
struct FInputActionValue;

/**
 *  Main Controllable PlayerCharacter
 */
UCLASS(abstract)
class ACitySimulatorDemoCharacter : public ACharacter
{
	GENERATED_BODY()

private: 
	//Player Controller
	UPROPERTY(BlueprintReadOnly, Category = "PlayerController", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACitySimulatorDemoPlayerController> MyPlayerController;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true", DisplayName = "CDInteractionManager"))
	TObjectPtr<UCDInteractionManager> CDInteractionManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true", DisplayName = "CDUserInterfaceManager"))
	TObjectPtr<UCDUserInterfaceManager> CDUserInterfaceManager;

protected:
	// Move Input Action 
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;
	// Mouse Look Input Action
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;
	// Sprint Input Action
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;
	// Interact Input Action
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

public:
	//Getters
	UFUNCTION(BlueprintPure, Category = "CD|Getters", meta = (CompactNodeTitle = "Get Player Controller"))
	ACitySimulatorDemoPlayerController* GetPlayerController() const;

	UFUNCTION(BlueprintCallable, Category="CD|Character")
	void SetNewInputEnabled(bool bNewEnabled);

protected:

	//BeginPlay
	virtual void BeginPlay() override;

	// Initialize input action bindings
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called for movement input 
	void Move(const FInputActionValue& Value);

	// Called for looking input 
	void Look(const FInputActionValue& Value);

	// Called for sprint toggle
	void Sprint(const FInputActionValue& Value);

	// Called for interaction input
	void Interact(const FInputActionValue& Value);

public:
	// Constructor 
	ACitySimulatorDemoCharacter();	

	//Components
	void InitializeComponents();

	//Component Getters
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class UCDInteractionManager* GetInteractionManager() const { return CDInteractionManager; }
	FORCEINLINE class UCDUserInterfaceManager* GetUserInterfaceManager() const { return CDUserInterfaceManager; }
};

