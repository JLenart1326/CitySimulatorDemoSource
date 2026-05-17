#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CDComponents.generated.h"

class ACitySimulatorDemoCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCDComponents : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for managing the player's core components
 */
class CITYSIMULATORDEMO_API ICDComponents
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CDComponents")
	void ComponentInit(ACitySimulatorDemoCharacter* Owner);
	
public:
};