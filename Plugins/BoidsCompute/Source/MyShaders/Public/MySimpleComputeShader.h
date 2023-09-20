#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "MySimpleComputeShader.generated.h"

struct MYSHADERS_API FMySimpleComputeShaderDispatchParams
{
	int X;
	int Y;
	int Z;


	int Input[2];
	int Output;

	TArray<FBoidProps> BoidProps;
	int numBoids;
	float viewRadius;
	float avoidRadius;


	FMySimpleComputeShaderDispatchParams(int x, int y, int z)
		: X(x)
		, Y(y)
		, Z(z)
	{
	}
};
USTRUCT(BlueprintType)
struct FBoidProps {

	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Variables")
		FVector position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Variables")
		FVector direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Variables")
		FVector flockHeading;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Variables")
		FVector flockCentre;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Variables")
		FVector avoidanceHeading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Variables")
	int numFlockmates;



};

// This is a public interface that we define so outside code can invoke our compute shader.
class MYSHADERS_API FMySimpleComputeShaderInterface {
public:
	// Executes this shader on the render thread
	static void DispatchRenderThread(
		FRHICommandListImmediate& RHICmdList,
		FMySimpleComputeShaderDispatchParams Params,
		TFunction<void(int OutputVal, TArray<FBoidProps>)> AsyncCallback
	);

	// Executes this shader on the render thread from the game thread via EnqueueRenderThreadCommand
	static void DispatchGameThread(
		FMySimpleComputeShaderDispatchParams Params,
		TFunction<void(int OutputVal, TArray<FBoidProps>)> AsyncCallback
	)
	{
		ENQUEUE_RENDER_COMMAND(SceneDrawCompletion)(
			[Params, AsyncCallback](FRHICommandListImmediate& RHICmdList)
			{
				DispatchRenderThread(RHICmdList, Params, AsyncCallback);
			});
	}

	// Dispatches this shader. Can be called from any thread
	static void Dispatch(
		FMySimpleComputeShaderDispatchParams Params,
		TFunction<void(int OutputVal, TArray<FBoidProps>)> AsyncCallback
	)
	{
		if (IsInRenderingThread()) {
			DispatchRenderThread(GetImmediateCommandList_ForRenderCommand(), Params, AsyncCallback);
		}
		else {
			DispatchGameThread(Params, AsyncCallback);
		}
	}
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMySimpleComputeShaderLibrary_AsyncExecutionCompleted, const int, Value, TArray<FBoidProps>, Value2);


UCLASS() // Change the _API to match your project
class MYSHADERS_API UMySimpleComputeShaderLibrary_AsyncExecution : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	// Execute the actual load
	virtual void Activate() override {
		// Create a dispatch parameters struct and fill it the input array with our args
		FMySimpleComputeShaderDispatchParams Params(1, 1, 1);
		Params.Input[0] = Arg1;
		Params.Input[1] = Arg2;
		Params.BoidProps = boidProps;
		Params.numBoids = numBoids;
		Params.viewRadius = viewRadius;
		Params.avoidRadius = avoidRadius;

		// Dispatch the compute shader and wait until it completes
		FMySimpleComputeShaderInterface::Dispatch(Params, [this](int OutputVal, TArray<FBoidProps> boidProps) {
			this->Completed.Broadcast(OutputVal, boidProps);
			int outputTmp = OutputVal;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("OutputVal=%d")));
				});
		
	}



	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "ComputeShader", WorldContext = "WorldContextObject"))
		static UMySimpleComputeShaderLibrary_AsyncExecution* ExecuteBaseComputeShader(UObject* WorldContextObject, int Arg1, int Arg2, TArray<FBoidProps> boidProps, int numBoids, int viewRadius, int avoidRadius) {
		UMySimpleComputeShaderLibrary_AsyncExecution* Action = NewObject<UMySimpleComputeShaderLibrary_AsyncExecution>();
		Action->Arg1 = Arg1;
		Action->Arg2 = Arg2;
		Action->boidProps = boidProps;
		Action->numBoids = numBoids;
		Action->viewRadius = viewRadius;
		Action->avoidRadius = avoidRadius;
		Action->RegisterWithGameInstance(WorldContextObject);

		return Action;
	}

	UPROPERTY(BlueprintAssignable)
		FOnMySimpleComputeShaderLibrary_AsyncExecutionCompleted Completed;


	int Arg1;
	int Arg2;
	TArray<FBoidProps> boidProps;
	int numBoids;
	float viewRadius;
	float avoidRadius;

};