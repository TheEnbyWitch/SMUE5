// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class libsm64 : ModuleRules
{
	public libsm64(ReadOnlyTargetRules Target) : base(Target)
	{
		// Force little endian
		PrivateDefinitions.Add("__ORDER_BIG_ENDIAN__=0");
		PrivateDefinitions.Add("__BYTE_ORDER__=1");
		PrivateDefinitions.Add("IS_BIG_ENDIAN=0");
		PrivateDefinitions.Add("VERSION_JP=0");
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Projects",
				// ... add other public dependencies that you statically link with here ...
			}
		);
	}
}
