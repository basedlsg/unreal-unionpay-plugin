#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "Templates/SharedPointer.h"
#include "UnionPaySecurityManager.generated.h"

/**
 * UnionPay Security Manager
 * Handles secure storage and retrieval of JWT tokens.
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "UnionPay Security Manager"))
class UNIONPAYSECURITY_API UUnionPaySecurityManager : public UObject
{
    GENERATED_BODY()

public:
    UUnionPaySecurityManager();
    virtual ~UUnionPaySecurityManager();

    /**
     * Saves the JWT token securely.
     * @param Token The JWT token to save.
     */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Security")
    void SaveToken(const FString& Token);

    /**
     * Retrieves the stored JWT token.
     * @return The stored JWT token, or an empty string if not found.
     */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Security")
    FString GetToken();

private:
    /** The key used to store the token in the credential manager. */
    const FString TokenStorageKey = TEXT("UnionPayJWT");
};