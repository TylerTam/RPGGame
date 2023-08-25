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

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EInputType : uint8
{
	NONE = 0 UMETA(Hidden),
	Movement = 1 << 0,
	Interactability = 1 << 1,
	InventoryQuickRow = 1 << 2,
	InventoryOpen = 1 << 3

};
ENUM_CLASS_FLAGS(EInputType);


class RPG_GAME_API CustomEnums
{
public:
	CustomEnums();
	~CustomEnums();
};
