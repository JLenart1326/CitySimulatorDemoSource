#include "CitySimulatorDemoFunctionLibrary.h"
#include "Core/CitySimulatorDemoCharacter.h"
#include "Core/CitySimulatorDemoPlayerController.h"
#include <Engine/LatentActionManager.h>
#include <WorldPartition/DataLayer/DataLayerManager.h>
#include <Kismet/GameplayStatics.h>

DEFINE_LOG_CATEGORY(LogCitySimulatorDemoFunctionLibrary);

void UCitySimulatorDemoFunctionLibrary::PrintError(FString Text)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Text, true);
}

void UCitySimulatorDemoFunctionLibrary::PrintWarning(FString Text)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, Text, true);
}

void UCitySimulatorDemoFunctionLibrary::PrintInfo(FString Text)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, Text, true);
}

void UCitySimulatorDemoFunctionLibrary::PrintSuccess(FString Text)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Text, true);
}

void UCitySimulatorDemoFunctionLibrary::LoadDataLayerLatent(UObject* WorldContext, FLatentActionInfo LatentInfo, ELoadDataLayerInputPins InputPins, const UDataLayerAsset* InDataLayerAsset, EDataLayerRuntimeState InState, ELoadDataLayerOutputPins& OutputPins, bool& bOutSuccess, FString& OutInfoMessage)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);

	if (World == nullptr)
	{
		bOutSuccess = false;
		OutInfoMessage = "Load Data Layer Latent Failed - World is not valid.";
		//Not outpin will be called, object does not exist
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutInfoMessage);
		return;
	}

	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

	//find existing action if it still running
	FLoadDataLayerLatent* ExistingAction = LatentActionManager.FindExistingAction<FLoadDataLayerLatent>(LatentInfo.CallbackTarget, LatentInfo.UUID);

	if (InputPins == ELoadDataLayerInputPins::Start)
	{
		if (ExistingAction == nullptr)
		{
			FLoadDataLayerLatent* Action = new FLoadDataLayerLatent(World, LatentInfo, InDataLayerAsset, InState, OutputPins,
				bOutSuccess, OutInfoMessage);
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, Action);
		}
	}
	else if (InputPins == ELoadDataLayerInputPins::Cancel)
	{
		if (ExistingAction != nullptr)
		{
			ExistingAction->bWantToCancel = true;
		}
	}
}

ACitySimulatorDemoCharacter* UCitySimulatorDemoFunctionLibrary::GetMainCharacter(const UObject* WorldContext)
{
	ACitySimulatorDemoCharacter* MainCharacter = Cast<ACitySimulatorDemoCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContext, 0));
	return MainCharacter;
}

ACitySimulatorDemoPlayerController* UCitySimulatorDemoFunctionLibrary::GetMainController(const UObject* WorldContext)
{
	ACitySimulatorDemoPlayerController* MainController = Cast<ACitySimulatorDemoPlayerController>(UGameplayStatics::GetPlayerController(WorldContext, 0));
	return MainController;
}

void FLoadDataLayerLatent::UpdateOperation(FLatentResponse& Response)
{
	if (bWantToCancel)
	{
		bSuccess = true;
		InfoMessage = "Load Data Layer Latent Succeeded - Cancelled by user.";

		Output = ELoadDataLayerOutputPins::OnCancelled;
		Response.FinishAndTriggerIf(true, LatentActionInfo.ExecutionFunction, LatentActionInfo.Linkage, LatentActionInfo.CallbackTarget);
		return;
	}

	if (bIsFirstCall)
	{
		bIsFirstCall = false;

		WorldPartitionSubsystem = World->GetSubsystem<UWorldPartitionSubsystem>();
		if (WorldPartitionSubsystem == nullptr)
		{
			bSuccess = false;
			InfoMessage = "Load Data Layer Latent Failed - WorldPartitionSubsystem is not valid.";
			Output = ELoadDataLayerOutputPins::OnFailed;
			Response.FinishAndTriggerIf(true, LatentActionInfo.ExecutionFunction, LatentActionInfo.Linkage, LatentActionInfo.CallbackTarget);
			return;
		}

		if (InDataLayerAsset == nullptr)
		{
			bSuccess = false;
			InfoMessage = "Load Data Layer Latent Failed - DataLayerAsset is not valid.";
			Output = ELoadDataLayerOutputPins::OnFailed;
			Response.FinishAndTriggerIf(true, LatentActionInfo.ExecutionFunction, LatentActionInfo.Linkage, LatentActionInfo.CallbackTarget);
			return;
		}

		UDataLayerManager* DataLayerManager = UDataLayerManager::GetDataLayerManager(World);
		if (DataLayerManager == nullptr)
		{
			bSuccess = false;
			InfoMessage = "Load Data Layer Latent Failed - DataLayerManager is not valid.";
			Output = ELoadDataLayerOutputPins::OnFailed;
			Response.FinishAndTriggerIf(true, LatentActionInfo.ExecutionFunction, LatentActionInfo.Linkage, LatentActionInfo.CallbackTarget);
			return;
		}

		DataLayerManager->SetDataLayerRuntimeState(InDataLayerAsset, InState, true);

		bSuccess = true;
		InfoMessage = "Load Data Layer Latent Started. . .";

		Output = ELoadDataLayerOutputPins::OnStarted;
		Response.TriggerLink(LatentActionInfo.ExecutionFunction, LatentActionInfo.Linkage, LatentActionInfo.CallbackTarget);
		return;
	}

	//is action done?
	bStreamingComplete = WorldPartitionSubsystem->IsStreamingCompleted();
	if (bStreamingComplete)
	{
		bSuccess = true;
		InfoMessage = "Load Data Layer Latent Completed";

		Output = ELoadDataLayerOutputPins::OnCompleted;
		Response.FinishAndTriggerIf(true, LatentActionInfo.ExecutionFunction, LatentActionInfo.Linkage, LatentActionInfo.CallbackTarget);
		return;
	}
}


