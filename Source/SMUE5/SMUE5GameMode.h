// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LibSM64Component.h"
#include "GameFramework/GameModeBase.h"
#include "SMUE5GameMode.generated.h"

UCLASS(BlueprintType)
class ASMUE5GameMode : public AGameModeBase
{
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	GENERATED_BODY()

public:
	ASMUE5GameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ULibSM64Component* LibSM64Comp;

	static ASMUE5GameMode *GetSMGameMode(UObject *WorldContext);

	ULibSM64Component* GetLibSM64Component() const { return LibSM64Comp; }
};



