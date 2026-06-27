// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Chapter3_3 : ModuleRules
{
	public Chapter3_3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Chapter3_3",
			"Chapter3_3/Variant_Horror",
			"Chapter3_3/Variant_Horror/UI",
			"Chapter3_3/Variant_Shooter",
			"Chapter3_3/Variant_Shooter/AI",
			"Chapter3_3/Variant_Shooter/UI",
			"Chapter3_3/Variant_Shooter/Weapons",
			"Chapter3_3/GAS",
			"Chapter3_3/GAS/Attributes",
			"Chapter3_3/GAS/Abilities",
			"Chapter3_3/GAS/Effects",
			"Chapter3_3/Items",
			"Chapter3_3/Wave"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
