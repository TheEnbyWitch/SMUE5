// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "libsm64inc.h"
#include "MarioComponent.generated.h"

USTRUCT(BlueprintType)
struct FSM64MarioInputs
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FRotator CamLook;
	UPROPERTY(BlueprintReadWrite)
	FVector2D Stick;
	UPROPERTY(BlueprintReadWrite)
	bool ButtonA;
	UPROPERTY(BlueprintReadWrite)
	bool ButtonB;
	UPROPERTY(BlueprintReadWrite)
	bool ButtonZ;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIBSM64UE_API UMarioComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMarioComponent();

	~UMarioComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void TickMario(FSM64MarioInputs &Inputs);

	UPROPERTY(BlueprintReadWrite)
	int32 MarioID;

	struct SM64MarioInputs inputs;
	struct SM64MarioState state;
	struct SM64MarioGeometryBuffers buffers; 
};
