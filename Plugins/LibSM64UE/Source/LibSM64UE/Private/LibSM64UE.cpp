// Copyright Epic Games, Inc. All Rights Reserved.

#include "LibSM64UE.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FLibSM64UEModule"

void FLibSM64UEModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("LibSM64UE")->GetBaseDir();
}

void FLibSM64UEModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLibSM64UEModule, LibSM64UE)
