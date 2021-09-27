// Fill out your copyright notice in the Description page of Project Settings.


#include "MarioComponent.h"
#include "libsm64inc.h"
#include "Kismet/KismetMathLibrary.h"

SM64MarioInputs ConvertToSM64Struct(FSM64MarioInputs inputs)
{
	FRotator YawOnlyRot(0.0f, inputs.CamLook.Yaw, 0.0f);
	FVector Forward = UKismetMathLibrary::GetForwardVector(YawOnlyRot);

	inputs.Stick.Normalize();

	return SM64MarioInputs{
		Forward.X,
		Forward.Y,
		inputs.Stick.Y,
		-inputs.Stick.X,
		inputs.ButtonA,
		inputs.ButtonB,
		inputs.ButtonZ
	};
}

// Sets default values for this component's properties
UMarioComponent::UMarioComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	buffers.position = (float*)FMemory::Malloc(sizeof(float) * SM64_GEO_MAX_TRIANGLES * 3 * 3);
	buffers.normal = (float*)FMemory::Malloc(sizeof(float) * SM64_GEO_MAX_TRIANGLES * 3 * 3);
	buffers.color = (float*)FMemory::Malloc(sizeof(float) * SM64_GEO_MAX_TRIANGLES * 3 * 3);
	buffers.uv = (float*)FMemory::Malloc(sizeof(float) * SM64_GEO_MAX_TRIANGLES * 3 * 2);
}

UMarioComponent::~UMarioComponent()
{
	FMemory::Free(buffers.position);
	FMemory::Free(buffers.normal);
	FMemory::Free(buffers.color);
	FMemory::Free(buffers.uv);
}


// Called when the game starts
void UMarioComponent::BeginPlay()
{
	Super::BeginPlay();

	FVector Location = GetOwner()->GetActorLocation();
	
	MarioID = sm64_mario_create(Location.X,Location.Y,Location.Z);
}

void UMarioComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	sm64_mario_delete(MarioID);
}


// Called every frame
void UMarioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMarioComponent::TickMario(FSM64MarioInputs& Inputs)
{
	FVector MarioPos = GetOwner()->GetActorLocation();

	inputs = ConvertToSM64Struct(Inputs);
	
	state.position[0] = MarioPos.X;
	state.position[1] = MarioPos.Z;
	state.position[2] = MarioPos.Y;
	
	sm64_mario_tick(MarioID, &inputs, &state, &buffers);
	
	MarioPos.X = state.position[0];
	MarioPos.Y = state.position[2];
	MarioPos.Z = state.position[1];

	float faceyaw = (state.faceAngle / PI) * 180.f;
	
	GetOwner()->SetActorLocation(MarioPos, false);
	//GetOwner()->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

