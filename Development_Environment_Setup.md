# UnionPay Unreal Engine Plugin - Development Environment Setup

## Current Environment Status

### ✅ Available Tools
- **Xcode**: Installed at `/Applications/Xcode.app/Contents/Developer`
- **Apple Clang**: Version 17.0.0 (clang-1700.0.13.5)
- **Target Architecture**: arm64-apple-darwin24.5.0 (Apple Silicon)
- **Epic Games Launcher**: Installed at `/Applications/Epic Games Launcher.app`

### ❌ Missing Components
- **Unreal Engine**: Not detected in standard locations
- **Unreal Engine Command Line Tools**: Not available in PATH

## Required Setup Steps

### 1. Unreal Engine Installation
```bash
# Option 1: Install via Epic Games Launcher (Recommended)
# 1. Open Epic Games Launcher
# 2. Navigate to Unreal Engine tab
# 3. Install Unreal Engine 5.4+ (latest stable)
# 4. Choose "Engine + Source Code" for plugin development

# Option 2: Build from Source (Advanced)
git clone https://github.com/EpicGames/UnrealEngine.git
cd UnrealEngine
./Setup.sh
./GenerateProjectFiles.sh
xcodebuild -project UnrealEngine.xcodeproj -target UE5 -configuration Development
```

### 2. Development Tools Setup
```bash
# Install Homebrew package manager
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install additional development tools
brew install cmake
brew install git-lfs
brew install nodejs npm  # For backend services

# Install Python for automation scripts
brew install python3
pip3 install requests cryptography
```

### 3. C++ Development Environment
```bash
# Verify Xcode command line tools
xcode-select --install

# Install additional C++ libraries
brew install openssl
brew install curl
brew install json-c

# Set up environment variables
echo 'export UE5_ROOT="/Users/Shared/Epic Games/UE_5.4"' >> ~/.zshrc
echo 'export PATH="$UE5_ROOT/Engine/Binaries/Mac:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

### 4. Plugin Development Structure
```bash
# Create plugin workspace
mkdir -p ~/UnrealPlugins/UnionPayPlugin
cd ~/UnrealPlugins/UnionPayPlugin

# Initialize plugin structure
mkdir -p Source/{UnionPayCore,UnionPayEditor,UnionPaySecurity,UnionPayUI}
mkdir -p Source/UnionPayCore/{Public,Private}
mkdir -p Source/UnionPayEditor/{Public,Private}
mkdir -p Source/UnionPaySecurity/{Public,Private}
mkdir -p Source/UnionPayUI/{Public,Private}
mkdir -p Config
mkdir -p Resources/{UI,Localization}
mkdir -p Content/{Blueprints,UI}
```

## Plugin Architecture Overview

### Module Structure
```
UnionPayPlugin/
├── UnionPayPlugin.uplugin          # Plugin descriptor
├── Source/
│   ├── UnionPayCore/               # Core payment logic (Runtime)
│   │   ├── Public/
│   │   │   ├── UnionPayManager.h
│   │   │   ├── PaymentTypes.h
│   │   │   └── UnionPayCore.h
│   │   ├── Private/
│   │   │   ├── UnionPayManager.cpp
│   │   │   ├── PaymentProcessor.cpp
│   │   │   └── UnionPayCore.cpp
│   │   └── UnionPayCore.Build.cs
│   ├── UnionPayEditor/             # Editor tools (Editor)
│   │   ├── Public/
│   │   │   └── UnionPayEditorModule.h
│   │   ├── Private/
│   │   │   ├── UnionPayEditorModule.cpp
│   │   │   └── PaymentConfigWidget.cpp
│   │   └── UnionPayEditor.Build.cs
│   ├── UnionPaySecurity/           # Security & encryption
│   │   ├── Public/
│   │   │   ├── SecurityManager.h
│   │   │   └── EncryptionUtils.h
│   │   ├── Private/
│   │   │   ├── SecurityManager.cpp
│   │   │   └── EncryptionUtils.cpp
│   │   └── UnionPaySecurity.Build.cs
│   └── UnionPayUI/                 # UI components
│       ├── Public/
│       │   ├── PaymentWidget.h
│       │   └── UnionPayUI.h
│       ├── Private/
│       │   ├── PaymentWidget.cpp
│       │   └── UnionPayUI.cpp
│       └── UnionPayUI.Build.cs
├── Config/
│   └── DefaultUnionPay.ini
└── Resources/
    ├── UI/
    │   ├── payment_icons/
    │   └── ui_assets/
    └── Localization/
        ├── en/
        ├── zh/
        └── ja/
```

## Development Workflow

### 1. Plugin Creation
```bash
# Generate plugin template using UE5 tools
cd "$UE5_ROOT/Engine/Binaries/DotNET/UnrealBuildTool"
./UnrealBuildTool -projectfiles -project="UnionPayPlugin.uproject" -game -rocket -progress
```

### 2. Build System Configuration
```cpp
// UnionPayCore.Build.cs
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
            "SSL"
        });
        
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Slate",
            "SlateCore",
            "UMG",
            "ToolMenus"
        });
        
        // Security libraries
        if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicFrameworks.Add("Security");
            PublicFrameworks.Add("CommonCrypto");
        }
    }
}
```

### 3. Testing Environment
```bash
# Create test project
mkdir -p ~/UnrealProjects/UnionPayTest
cd ~/UnrealProjects/UnionPayTest

# Generate test project files
"$UE5_ROOT/Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool" -projectfiles -project="UnionPayTest.uproject" -game -rocket -progress
```

## Next Steps

1. **Install Unreal Engine 5.4+** via Epic Games Launcher
2. **Verify installation** by creating a new C++ project
3. **Set up plugin development workspace** with proper directory structure
4. **Configure build environment** with required dependencies
5. **Create basic plugin template** and test compilation

## Verification Commands

```bash
# Verify Unreal Engine installation
ls "$UE5_ROOT/Engine/Binaries/Mac/"

# Test plugin compilation
cd ~/UnrealPlugins/UnionPayPlugin
"$UE5_ROOT/Engine/Build/BatchFiles/Mac/Build.sh" UnionPayCore Development Mac

# Verify development tools
which clang
which cmake
which node
which python3
```

## Troubleshooting

### Common Issues
1. **Xcode Version Compatibility**: Ensure Xcode 14+ for UE5 compatibility
2. **Apple Silicon Compatibility**: Verify arm64 target architecture
3. **Plugin Dependencies**: Check all required modules are available
4. **Build Path Issues**: Ensure UE5_ROOT environment variable is set correctly

This setup provides a comprehensive foundation for UnionPay plugin development with Unreal Engine on macOS with Apple Silicon.