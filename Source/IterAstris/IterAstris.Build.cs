// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IterAstris : ModuleRules
{
    public IterAstris(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] { 
            "IterAstris/Public/Animations",
            "IterAstris/Public/Player",
            "IterAstris/Public/Components",
            "IterAstris/Public/Components/Player",
            "IterAstris/Public/DamageZones",
            "IterAstris/Public/Weapons",
            "IterAstris/Public/UI",
            "IterAstris/Public/DamageTypes"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
