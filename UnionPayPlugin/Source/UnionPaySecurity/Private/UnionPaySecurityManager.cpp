#include "UnionPaySecurityManager.h"
#include "HAL/PlatformProcess.h"
#include "Misc/SecureHash.h"
#include "GenericPlatform/GenericPlatformCredential.h"

UUnionPaySecurityManager::UUnionPaySecurityManager()
{
}

UUnionPaySecurityManager::~UUnionPaySecurityManager()
{
}

void UUnionPaySecurityManager::SaveToken(const FString& Token)
{
    FPlatformCredentialManager::Get().Store(TokenStorageKey, FPlatformCredential(FPlatformProcess::UserName(), Token));
}

FString UUnionPaySecurityManager::GetToken()
{
    FPlatformCredential Credential = FPlatformCredentialManager::Get().Retrieve(TokenStorageKey);
    return Credential.Password;
}