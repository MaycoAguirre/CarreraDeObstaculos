// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CarreraDeObstaculos : ModuleRules
{
	public CarreraDeObstaculos(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"CarreraDeObstaculos",
			"CarreraDeObstaculos/Variant_Platforming",
			"CarreraDeObstaculos/Variant_Platforming/Animation",
			"CarreraDeObstaculos/Variant_Combat",
			"CarreraDeObstaculos/Variant_Combat/AI",
			"CarreraDeObstaculos/Variant_Combat/Animation",
			"CarreraDeObstaculos/Variant_Combat/Gameplay",
			"CarreraDeObstaculos/Variant_Combat/Interfaces",
			"CarreraDeObstaculos/Variant_Combat/UI",
			"CarreraDeObstaculos/Variant_SideScrolling",
			"CarreraDeObstaculos/Variant_SideScrolling/AI",
			"CarreraDeObstaculos/Variant_SideScrolling/Gameplay",
			"CarreraDeObstaculos/Variant_SideScrolling/Interfaces",
			"CarreraDeObstaculos/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
