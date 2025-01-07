// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class timbermvp : ModuleRules
{
	public timbermvp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore",
			"EnhancedInput", "UMG", "SlateCore", "Slate"
		});
	}
}