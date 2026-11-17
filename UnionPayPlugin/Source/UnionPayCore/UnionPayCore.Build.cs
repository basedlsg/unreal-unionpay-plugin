using UnrealBuildTool;

public class UnionPayCore : ModuleRules
{
    public UnionPayCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "HTTP",
            "Json",
            "JsonObjectConverter",
            "Slate",
            "SlateCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Projects",
            "ToolMenus",
            "UnionPaySecurity",
            "JsonUtilities"
        });

        // Platform-specific dependencies
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicSystemLibraries.Add("Crypt32.lib");
            PublicSystemLibraries.Add("Ws2_32.lib");
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicFrameworks.Add("Security");
            PublicFrameworks.Add("Foundation");
            PublicFrameworks.Add("SystemConfiguration");
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            PublicSystemLibraries.Add("ssl");
            PublicSystemLibraries.Add("crypto");
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PublicSystemLibraries.Add("ssl");
            PublicSystemLibraries.Add("crypto");
            
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", System.IO.Path.Combine(PluginPath, "UnionPayPlugin_APL.xml"));
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PublicFrameworks.Add("Security");
            PublicFrameworks.Add("Foundation");
            PublicFrameworks.Add("CFNetwork");
        }

        // Enable RTTI for payment processing
        bUseRTTI = true;

        // Definitions
        PublicDefinitions.Add("UNIONPAY_CORE_MODULE=1");
        
        if (Target.Configuration == UnrealTargetConfiguration.Development || 
            Target.Configuration == UnrealTargetConfiguration.Debug)
        {
            PublicDefinitions.Add("UNIONPAY_DEBUG=1");
        }

        // Include paths
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