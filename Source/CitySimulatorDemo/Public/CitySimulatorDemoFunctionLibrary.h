#pragma once

#include "CoreMinimal.h"
#include <Kismet/BlueprintFunctionLibrary.h>
#include <LatentActions.h>
#include <WorldPartition/DataLayer/DataLayerInstance.h>
#include <WorldPartition/WorldPartitionSubsystem.h>
#include "CitySimulatorDemoFunctionLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCitySimulatorDemoFunctionLibrary, Warning, All);

class ACitySimulatorDemoCharacter;
class ACitySimulatorDemoPlayerController;

/**
 * Base Function Library with functions accessible from Blueprints
 */
UENUM()
enum class ELoadDataLayerInputPins : uint8
{
	Start,
	Cancel
};

UENUM()
enum class ELoadDataLayerOutputPins : uint8
{
	OnStarted,
	OnCompleted,
	OnFailed,
	OnCancelled
};

UCLASS()
class CITYSIMULATORDEMO_API UCitySimulatorDemoFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//Prints
	UFUNCTION(BlueprintCallable, Category = "FL|Prints")
	static void PrintError(FString Text);
	UFUNCTION(BlueprintCallable, Category = "FL|Prints")
	static void PrintWarning(FString Text);
	UFUNCTION(BlueprintCallable, Category = "FL|Prints")
	static void PrintInfo(FString Text);
	UFUNCTION(BlueprintCallable, Category = "FL|Prints")
	static void PrintSuccess(FString Text);

	//DataLayerAsyncLoad
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", Latent, LatentInfo = "LatentInfo", ExpandEnumAsExecs = "InputPins,OutputPins"), Category = "FL|DataLayers")
	static void LoadDataLayerLatent(UObject* WorldContext, FLatentActionInfo LatentInfo, ELoadDataLayerInputPins InputPins, const UDataLayerAsset* InDataLayerAsset,
		EDataLayerRuntimeState InState, ELoadDataLayerOutputPins& OutputPins, bool& bOutSuccess, FString& OutInfoMessage);

	//Get Player Character
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext"), Category = "FL|Player")
	static ACitySimulatorDemoCharacter* GetMainCharacter(const UObject* WorldContext);

	//Get Player Controller
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext"), Category = "FL|Player")
	static ACitySimulatorDemoPlayerController* GetMainController(const UObject* WorldContext);
};

class FLoadDataLayerLatent : public FPendingLatentAction
{
public:
	bool& bSuccess;
	FString& InfoMessage;

	bool bIsFirstCall = true;
	bool bWantToCancel = false;

	const UWorld* World;
	UWorldPartitionSubsystem* WorldPartitionSubsystem = nullptr;
	bool bStreamingComplete = false;
	EDataLayerRuntimeState InState = EDataLayerRuntimeState::Activated;
	const UDataLayerAsset* InDataLayerAsset;

	//Information required to complete the latent action properly
	FLatentActionInfo LatentActionInfo;

	//Output pins
	ELoadDataLayerOutputPins& Output;

public:
	//Constructor
	FLoadDataLayerLatent(const UWorld* World, FLatentActionInfo LatentInfo,
		const UDataLayerAsset* InDataLayerAsset, EDataLayerRuntimeState InState, ELoadDataLayerOutputPins& OutputPins, bool& bOutSuccess, FString& OutInfoMessage)
		: World(World),
		LatentActionInfo(LatentInfo),
		InDataLayerAsset(InDataLayerAsset),
		InState(InState),
		Output(OutputPins),
		bSuccess(bOutSuccess),
		InfoMessage(OutInfoMessage)
	{
		Output = ELoadDataLayerOutputPins::OnStarted;
		bSuccess = false;
		InfoMessage = "";
	}

	virtual void UpdateOperation(FLatentResponse& Response) override;
};
