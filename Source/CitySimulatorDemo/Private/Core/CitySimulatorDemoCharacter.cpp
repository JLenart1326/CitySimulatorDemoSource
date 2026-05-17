#include "Core/CitySimulatorDemoCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <Kismet/GameplayStatics.h>
#include "Core/CitySimulatorDemoPlayerController.h"
#include "Core/CitySimulatorDemo.h"
#include "Core/Components/CDInteractionManager.h"
#include "Core/Components/CDUserInterfaceManager.h"
#include "Core/Interfaces/CDComponents.h"

ACitySimulatorDemoCharacter::ACitySimulatorDemoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//Create Interaction Manager Component
	CDInteractionManager = CreateDefaultSubobject<UCDInteractionManager>(TEXT("InteractionManager"));

	//Create User Interface Manager Component
	CDUserInterfaceManager = CreateDefaultSubobject<UCDUserInterfaceManager>(TEXT("UserInterfaceManager"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ACitySimulatorDemoCharacter::InitializeComponents()
{
	if (ensure(CDInteractionManager) && ensure(CDInteractionManager->Implements<UCDComponents>())) {
		ICDComponents::Execute_ComponentInit(CDInteractionManager, this);
	}

	if (ensure(CDUserInterfaceManager) && ensure(CDUserInterfaceManager->Implements<UCDComponents>())) {
		ICDComponents::Execute_ComponentInit(CDUserInterfaceManager, this);
	}
}

ACitySimulatorDemoPlayerController* ACitySimulatorDemoCharacter::GetPlayerController() const
{
	return MyPlayerController;
}

void ACitySimulatorDemoCharacter::SetNewInputEnabled(bool bNewEnabled)
{
	if (bNewEnabled) {
		if (!this->InputEnabled()) {
			this->EnableInput(MyPlayerController);
		}
	}
	else {
		if (this->InputEnabled()) {
			this->DisableInput(MyPlayerController);
		}
	}
}

void ACitySimulatorDemoCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Fetching PlayerController
	MyPlayerController = Cast<ACitySimulatorDemoPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (MyPlayerController == nullptr)
	{
		UE_LOG(LogCitySimulatorDemo, Error, TEXT("ACitySimulatorDemoPlayerController not found!"));
	}

	//Components init
	InitializeComponents();
}

void ACitySimulatorDemoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACitySimulatorDemoCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ACitySimulatorDemoCharacter::Look);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ACitySimulatorDemoCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACitySimulatorDemoCharacter::Sprint);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACitySimulatorDemoCharacter::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ACitySimulatorDemoCharacter::Interact);
	}
	else
	{
		UE_LOG(LogCitySimulatorDemo, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACitySimulatorDemoCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	if (MyPlayerController != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = MyPlayerController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACitySimulatorDemoCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	if (MyPlayerController != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACitySimulatorDemoCharacter::Sprint(const FInputActionValue& Value)
{

	//UE_LOG(LogTemp, Warning, TEXT("Value: %d"), Value.Get<bool>());
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp)
	{
		if (Value.GetValueType() != EInputActionValueType::Boolean) 
		{
			UE_LOG(LogTemp, Error, TEXT("Sprint: InputAction Value is invalid"));
		}
		MoveComp->MaxWalkSpeed = Value.Get<bool>() ? 500.0f : 200.0f;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Sprint: Character Movement is missing"));
	}
}

void ACitySimulatorDemoCharacter::Interact(const FInputActionValue& Value)
{
	if (Value.GetValueType() != EInputActionValueType::Boolean)
	{
		UE_LOG(LogTemp, Error, TEXT("Interact: InputAction Value is invalid"));
	}

	if (!ensureMsgf(CDInteractionManager != nullptr, TEXT("CDInteractionManagerComponent is not properly initialized for some reason"))) {
		return;
	}

	if (Value.Get<bool>()) {
		CDInteractionManager->RequestStartInteraction();
	}
	else {
		CDInteractionManager->RequestStopInteraction();
	}

}


