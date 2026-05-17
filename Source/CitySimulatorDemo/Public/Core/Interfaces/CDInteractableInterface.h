

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CDInteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCDInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class ACitySimulatorDemoCharacter;

/**
 * Interface for interactable objects
 */
class CITYSIMULATORDEMO_API ICDInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="CD|Interactions")
	void OnInteractionStarted(ACitySimulatorDemoCharacter* Interactor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CD|Interactions")
	void OnInteractionStopped(ACitySimulatorDemoCharacter* Interactor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CD|Interactions")
	bool IsHoldable();
};
