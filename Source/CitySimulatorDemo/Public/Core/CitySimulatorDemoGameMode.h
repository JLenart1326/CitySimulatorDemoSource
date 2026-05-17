#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CitySimulatorDemoGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class ACitySimulatorDemoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	ACitySimulatorDemoGameMode();
};



