// Shoot Them Up Game, All Right Reserved.

using UnrealBuildTool;

public class STU : ModuleRules
{
    public STU(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        //при билде поиск #include в указанных папках
        PublicIncludePaths.AddRange(new string[]
        {
            "STU/Character",
            "STU/Dev",
            "STU/Draft",
            "STU/GameModes",
            "STU/Input",
            "STU/Player",
            "STU/UI"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
