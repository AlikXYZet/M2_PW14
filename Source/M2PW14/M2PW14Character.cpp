// Copyright Epic Games, Inc. All Rights Reserved.

// Base:
#include "M2PW14Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// UE:
#include "GameplayEffect.h"

// GAS:
#include "M2PW14/GAS/GAS_AbilitySystemComponent.h"
#include "M2PW14/GAS/GAS_AttributeSet.h"
#include "M2PW14/GAS/GAS_GameplayAbility.h"
//--------------------------------------------------------------------------------------



//////////////////////////////////////////////////////////////////////////
// AM2PW14Character

AM2PW14Character::AM2PW14Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)



	/* ---   GAS   --- */

	AbilitySystemComponent = CreateDefaultSubobject<UGAS_AbilitySystemComponent>(TEXT("AbilitySystemComp"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSet = CreateDefaultSubobject<UGAS_AttributeSet>(TEXT("Attributes"));
	//-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   GAS   --- */

UAbilitySystemComponent* AM2PW14Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AM2PW14Character::InitAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle lEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		lEffectContextHandle.AddSourceObject(this);

		FGameplayEffectSpecHandle lSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			DefaultAttributeEffect,
			1,
			lEffectContextHandle);

		if (lSpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*lSpecHandle.Data.Get());
		}
	}
}

void AM2PW14Character::InitDefaultAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGAS_GameplayAbility>& Ability : DefaultAbilities)
		{
			int32 InputID = static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID);
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, InputID, this));
		}
	}
}

void AM2PW14Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitAttributes();
	InitDefaultAbilities();
}

void AM2PW14Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitAttributes();

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds InputBinds(
			"Confirm",
			"Cancel",
			"EGAS_AbilityInputID",
			static_cast<int32>(EGAS_AbilityInputID::Confirm),
			static_cast<int32>(EGAS_AbilityInputID::Cancel)
		);

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, InputBinds);
	}
}

//--------------------------------------------------------------------------------------



//////////////////////////////////////////////////////////////////////////
// Input

void AM2PW14Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AM2PW14Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AM2PW14Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AM2PW14Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AM2PW14Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AM2PW14Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AM2PW14Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AM2PW14Character::OnResetVR);



	/* ---   GAS   --- */

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds InputBinds(
			"Confirm",
			"Cancel",
			"EGAS_AbilityInputID",
			static_cast<int32>(EGAS_AbilityInputID::Confirm),
			static_cast<int32>(EGAS_AbilityInputID::Cancel)
		);

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, InputBinds);
	}
	//-------------------------------------------
}


void AM2PW14Character::OnResetVR()
{
	// If M2PW14 is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in M2PW14.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AM2PW14Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AM2PW14Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AM2PW14Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AM2PW14Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AM2PW14Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AM2PW14Character::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
