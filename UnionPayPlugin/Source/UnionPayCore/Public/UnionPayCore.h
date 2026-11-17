#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Containers/UnrealString.h"
#include "Templates/SharedPointer.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUnionPay, Log, All);

class FUnionPayManager;
class FUnionPayNetworkManager;
class FUnionPayCacheManager;

/**
 * UnionPay Core Module
 * Provides core payment processing functionality for UnionPay integration
 */
class UNIONPAYCORE_API FUnionPayCoreModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    /** Singleton getter for the module */
    static FUnionPayCoreModule& Get();

    /** Check if the module is loaded */
    static bool IsAvailable();

    /** Get the UnionPay manager instance */
    TSharedPtr<FUnionPayManager> GetPaymentManager() const;

    /** Get the network manager instance */
    TSharedPtr<FUnionPayNetworkManager> GetNetworkManager() const;

    /** Get the cache manager instance */
    TSharedPtr<FUnionPayCacheManager> GetCacheManager() const;

    /** Initialize the payment system with configuration */
    bool Initialize(const FString& APIEndpoint, const FString& APIKey, bool bSandboxMode = true);

    /** Shutdown the payment system */
    void Shutdown();

    /** Check if the payment system is initialized */
    bool IsInitialized() const;

    /** Get plugin version */
    FString GetPluginVersion() const;

    /** Get supported payment methods */
    TArray<FString> GetSupportedPaymentMethods() const;

    /** Get supported currencies */
    TArray<FString> GetSupportedCurrencies() const;

private:
    /** Payment manager instance */
    TSharedPtr<FUnionPayManager> PaymentManager;

    /** Network manager instance */
    TSharedPtr<FUnionPayNetworkManager> NetworkManager;

    /** Cache manager instance */
    TSharedPtr<FUnionPayCacheManager> CacheManager;

    /** Module initialization state */
    bool bIsInitialized;

    /** Sandbox mode flag */
    bool bSandboxMode;

    /** API configuration */
    FString APIEndpoint;
    FString APIKey;
};

/** 
 * Convenience macros for accessing the UnionPay module
 */
#define UNIONPAY_MODULE_NAME "UnionPayCore"

/** Get the UnionPay module */
#define GET_UNIONPAY_MODULE() FUnionPayCoreModule::Get()

/** Get the payment manager */
#define GET_UNIONPAY_MANAGER() FUnionPayCoreModule::Get().GetPaymentManager()

/** Get the network manager */
#define GET_UNIONPAY_NETWORK() FUnionPayCoreModule::Get().GetNetworkManager()

/** Get the cache manager */
#define GET_UNIONPAY_CACHE() FUnionPayCoreModule::Get().GetCacheManager()

/** Check if UnionPay is available */
#define IS_UNIONPAY_AVAILABLE() FUnionPayCoreModule::IsAvailable()

/** UnionPay logging macros */
#define UE_LOG_UNIONPAY(Verbosity, Format, ...) \
    UE_LOG(LogUnionPay, Verbosity, Format, ##__VA_ARGS__)

#define UE_LOG_UNIONPAY_WARNING(Format, ...) \
    UE_LOG(LogUnionPay, Warning, Format, ##__VA_ARGS__)

#define UE_LOG_UNIONPAY_ERROR(Format, ...) \
    UE_LOG(LogUnionPay, Error, Format, ##__VA_ARGS__)

/** 
 * UnionPay API constants
 */
namespace UnionPayConstants
{
    // API Version
    static const FString API_VERSION = TEXT("v1");
    
    // Endpoints
    static const FString ENDPOINT_PAYMENTS = TEXT("/payments");
    static const FString ENDPOINT_TRANSACTIONS = TEXT("/transactions");
    static const FString ENDPOINT_PAYMENT_METHODS = TEXT("/payment-methods");
    static const FString ENDPOINT_WEBHOOKS = TEXT("/webhooks");
    
    // Payment Methods
    static const FString PAYMENT_METHOD_UNIONPAY = TEXT("unionpay");
    static const FString PAYMENT_METHOD_VISA = TEXT("visa");
    static const FString PAYMENT_METHOD_MASTERCARD = TEXT("mastercard");
    static const FString PAYMENT_METHOD_AMEX = TEXT("amex");
    
    // Currencies
    static const FString CURRENCY_USD = TEXT("USD");
    static const FString CURRENCY_EUR = TEXT("EUR");
    static const FString CURRENCY_CNY = TEXT("CNY");
    static const FString CURRENCY_JPY = TEXT("JPY");
    static const FString CURRENCY_GBP = TEXT("GBP");
    static const FString CURRENCY_CAD = TEXT("CAD");
    static const FString CURRENCY_AUD = TEXT("AUD");
    static const FString CURRENCY_SGD = TEXT("SGD");
    static const FString CURRENCY_HKD = TEXT("HKD");
    
    // Payment Status
    static const FString STATUS_PENDING = TEXT("pending");
    static const FString STATUS_PROCESSING = TEXT("processing");
    static const FString STATUS_COMPLETED = TEXT("completed");
    static const FString STATUS_FAILED = TEXT("failed");
    static const FString STATUS_CANCELLED = TEXT("cancelled");
    static const FString STATUS_REFUNDED = TEXT("refunded");
    
    // HTTP Headers
    static const FString HEADER_AUTHORIZATION = TEXT("Authorization");
    static const FString HEADER_CONTENT_TYPE = TEXT("Content-Type");
    static const FString HEADER_ACCEPT = TEXT("Accept");
    static const FString HEADER_USER_AGENT = TEXT("User-Agent");
    static const FString HEADER_API_KEY = TEXT("X-API-Key");
    static const FString HEADER_REQUEST_ID = TEXT("X-Request-ID");
    
    // Content Types
    static const FString CONTENT_TYPE_JSON = TEXT("application/json");
    static const FString CONTENT_TYPE_FORM = TEXT("application/x-www-form-urlencoded");
    
    // Timeout values (in seconds)
    static const float REQUEST_TIMEOUT = 30.0f;
    static const float PAYMENT_TIMEOUT = 300.0f;
    static const float WEBHOOK_TIMEOUT = 10.0f;
    
    // Cache TTL values (in seconds)
    static const int32 CACHE_TTL_SESSION = 3600;
    static const int32 CACHE_TTL_PAYMENT_METHODS = 1800;
    static const int32 CACHE_TTL_TRANSACTION_STATUS = 300;
    
    // Retry configuration
    static const int32 MAX_RETRY_ATTEMPTS = 3;
    static const float RETRY_DELAY_BASE = 1.0f;
    static const float RETRY_DELAY_MULTIPLIER = 2.0f;
    
    // Validation limits
    static const double MIN_PAYMENT_AMOUNT = 0.01;
    static const double MAX_PAYMENT_AMOUNT = 100000.0;
}