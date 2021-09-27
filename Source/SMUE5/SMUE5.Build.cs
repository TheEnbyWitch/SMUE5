// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SMUE5 : ModuleRules
{
	public SMUE5(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "LibSM64UE", "ProceduralMeshComponent", "libsm64" });
		PrivateDependencyModuleNames.Add("LibSM64UE");
	}
}
