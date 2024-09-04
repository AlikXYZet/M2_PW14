// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Base:
#include "CoreMinimal.h"

// GAS:
#include "Abilities/GameplayAbility.h"

// Interaction:
#include "M2PW14/M2PW14.h"

// Generated:
#include "GAS_GameplayAbility.generated.h"
//--------------------------------------------------------------------------------------



/**
 * 
 */
UCLASS()
class M2PW14_API UGAS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGAS_GameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGAS_AbilityInputID AbilityInputID = EGAS_AbilityInputID::None;
};
