// Copyright Epic Games, Inc. All Rights Reserved.

#include "M2PW14GameMode.h"
#include "M2PW14Character.h"
#include "UObject/ConstructorHelpers.h"

AM2PW14GameMode::AM2PW14GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
