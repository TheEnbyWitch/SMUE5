// Copyright Epic Games, Inc. All Rights Reserved.

#include "SMUE5GameMode.h"
#include "SMUE5Character.h"
#include "Engine/StaticMeshActor.h"
#include "UObject/ConstructorHelpers.h"

void ASMUE5GameMode::BeginPlay()
{
	Super::BeginPlay();

	LibSM64Comp->SM64_GlobalInit("sm64.z64");
	LibSM64Comp->SM64_BuildCollision();

	/*
	for (FThreadSafeObjectIterator Itr(AStaticMeshActor::StaticClass()); Itr; ++Itr)
	{
		if (Itr->IsA(AStaticMeshActor::StaticClass()))
		{
			FTriMeshCollisionData CollisionData;
			((AStaticMeshActor*)*Itr)->GetStaticMeshComponent()->GetStaticMesh()->GetPhysicsTriMeshData(&CollisionData, true);
		}
	}*/

}

void ASMUE5GameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	LibSM64Comp->SM64_GlobalTerminate();
}

ASMUE5GameMode::ASMUE5GameMode()
{
	LibSM64Comp = CreateDefaultSubobject<ULibSM64Component>(TEXT("libsm64_Component"));
	
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/

	DefaultPawnClass = ASMUE5Character::StaticClass();
}

ASMUE5GameMode* ASMUE5GameMode::GetSMGameMode(UObject* WorldContext)
{
	if(WorldContext && WorldContext->GetWorld())
	{
		return WorldContext->GetWorld()->GetAuthGameMode<ASMUE5GameMode>();
	}
	else
	{
		return nullptr;
	}
}
