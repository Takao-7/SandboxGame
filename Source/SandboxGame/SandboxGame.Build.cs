// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SandboxGame : ModuleRules
{
	public SandboxGame(ReadOnlyTargetRules Target) : base(Target)
	{
		CppStandard = CppStandardVersion.Cpp17;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay",
			"UnrealEngineECS",
			"Water",
			"Voxel"
		});
	}
}
