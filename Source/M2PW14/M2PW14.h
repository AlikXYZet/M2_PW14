// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGAS_AbilityInputID : uint8
{
	None,		// Требуется для GAS
	Confirm,	// Требуется для GAS
	Cancel,		// Требуется для GAS
	Healing,	// Лечение
	Blocking,	// Блокировка урона
};
