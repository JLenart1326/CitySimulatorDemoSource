#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Interfaces/CDComponents.h"
#include "CDInteractionManager.generated.h"

class ACitySimulatorDemoCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CITYSIMULATORDEMO_API UCDInteractionManager : public UActorComponent, public ICDComponents
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CD|Interaction")
	float InteractionRange = 300.f;

	UPROPERTY(BlueprintReadOnly, Category="CD|Interaction")
	TWeakObjectPtr<AActor> InteractingActor;

private:
	//Owner
	TObjectPtr<ACitySimulatorDemoCharacter> OwnerCharacter;

public:	
	// Sets default values for this component's properties
	UCDInteractionManager();

	//ICDComponents Interface Implementation
	virtual void ComponentInit_Implementation(ACitySimulatorDemoCharacter* Owner) override;

	//Interaction input request handling
	void RequestStartInteraction();
	void RequestStopInteraction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
