#pragma once

#include "CoreMinimal.h"

#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"

#include "LibSM64Component.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLIBSM64, Log, All);

UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent) )
class LIBSM64UE_API ULibSM64Component : public UActorComponent
{
public:
	ULibSM64Component()
	{
	}

private:
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SM64_GlobalInit(FString PathToRom);
	
	UFUNCTION(BlueprintCallable)
	void SM64_GlobalTerminate();

	UFUNCTION(BlueprintCallable)
	void SM64_BuildCollision();

	UPROPERTY(BlueprintReadWrite, Transient)
	UTexture2D* ROMTexture;
};
