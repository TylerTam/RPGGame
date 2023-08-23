// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EEatType : uint8
{
	NONE = 0 UMETA(Hidden),
	MICROORGANISM = 1 << 0,
	MEAT = 1 << 1,
	PLANT = 1 << 2,
};
ENUM_CLASS_FLAGS(EEatType);

class RPG_GAME_API CustomEnums
{
public:
	CustomEnums();
	~CustomEnums();
};
