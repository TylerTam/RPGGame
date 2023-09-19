// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidHelper.h"
#include "Math/UnrealMathUtility.h"



TArray<FVector> UBoidHelper::directions;
bool UBoidHelper::directionsCreated;
// Sets default values for this component's properties
UBoidHelper::UBoidHelper()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	if (directionsCreated) return;
	// ...
	directions.SetNum(numViewDirections);
	float goldenRatio = (1 + FMath::Sqrt(5)) / 2;
	float angleIncrement = PI* 2 * goldenRatio;

	for (int i = 0; i < numViewDirections; i++) {
		float t = (float)i / numViewDirections;
		float inclination = FMath::Acos(1 - 2 * t);
		float azimuth = angleIncrement * i;

		float y = FMath::Sin(inclination) * FMath::Cos(azimuth);
		float z = FMath::Sin(inclination) * FMath::Sin(azimuth);
		float x = FMath::Cos(inclination);
		directions[i] = FVector(x, y, z);
	}

}

TArray<FVector> UBoidHelper::GetDirections() {
	return directions;
}

// Called when the game starts
void UBoidHelper::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBoidHelper::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

