using UnrealBuildTool;

public class UnionPaySecurity : ModuleRules
{
    public UnionPaySecurity(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "CredentialManager"
        });

        PublicIncludePaths.AddRange(new string[]
        {
            System.IO.Path.Combine(ModuleDirectory, "Public")
        });

        PrivateIncludePaths.AddRange(new string[]
        {
            System.IO.Path.Combine(ModuleDirectory, "Private")
        });
    }
}