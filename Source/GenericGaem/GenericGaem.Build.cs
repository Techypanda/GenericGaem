// Copyright Epic Games, Inc. All Rights Reserved.

using System.Collections.Generic;
using UnrealBuildTool;

public class GenericGaem : ModuleRules
{
    readonly static string projectName = "GenericGaem";
    readonly static string privatePrefix = $"{projectName}/Private";
    readonly static string publicPrefix = $"{projectName}/Public";
    static string[] FolderStructure(string prefix)
    {
        return [
            $"{prefix}",
            $"{prefix}/Actors",
            $"{prefix}/Actors/Gamemodes",
            $"{prefix}/Actors/Items",
            $"{prefix}/Actors/Roles",
            $"{prefix}/Actors/Characters",
            $"{prefix}/Actors/PlayerControllers",
            $"{prefix}/Actors/PlayerState",
            $"{prefix}/Actors/World",
            $"{prefix}/Structs",
            $"{prefix}/UI",
            $"{prefix}/Interfaces",
            $"{prefix}/Tests",
            $"{prefix}/Data"
        ];
    }
    public GenericGaem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(FolderStructure(privatePrefix));

        PrivateIncludePaths.AddRange(FolderStructure(publicPrefix));

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
