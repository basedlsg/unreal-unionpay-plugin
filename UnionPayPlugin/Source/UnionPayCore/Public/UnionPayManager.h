#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Http.h"
#include "PaymentTypes.h"
#include "Templates/SharedPointer.h"
#include "Delegates/Delegate.h"
#include "UObject/NoExportTypes.h"
#include "UnionPayManager.generated.h"

class FUnionPayNetworkManager;
class FUnionPayCacheManager;
#include "UnionPaySecurity/Public/UnionPaySecurityManager.h"

/**
 * Delegate declarations for UnionPay payment events
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPaymentCompleted, const FString&, PaymentId, const FUnionPayPaymentResponse&, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPaymentFailed, const FString&, PaymentId, const FUnionPayError&, Error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPaymentCancelled, const FString&, PaymentId, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPaymentStatusChanged, const FString&, PaymentId, EUnionPayPaymentStatus, NewStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRefundCompleted, const FString&, PaymentId, const FUnionPayRefundResponse&, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthenticationSuccess, const FString&, JWT);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthenticationFailure, const FString&, ErrorMessage);


/**
 * Main UnionPay Manager class
 * Handles all payment operations and provides Blueprint-accessible API
 */
UCLASS(BlueprintType, Blueprintable)
class UNIONPAYCORE_API UUnionPayManager : public UObject
{
    GENERATED_BODY()

public:
    UUnionPayManager();
    virtual ~UUnionPayManager();

    //~ Begin UObject Interface
    virtual void BeginDestroy() override;
    //~ End UObject Interface

    /** Initialize the payment manager */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Core")
    bool Initialize(const FString& APIEndpoint, bool bSandboxMode = true);

    /** Shutdown the payment manager */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Core")
    void Shutdown();

    /** Check if the manager is initialized */
    UFUNCTION(BlueprintPure, Category = "UnionPay|Core")
    bool IsInitialized() const;

    /** Get the current configuration */
    UFUNCTION(BlueprintPure, Category = "UnionPay|Core")
    FUnionPayConfig GetConfiguration() const;

    /** Update configuration */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Core")
    bool UpdateConfiguration(const FUnionPayConfig& NewConfig);

    // ========================================================================
    // Authentication
    // ========================================================================

    /** Authenticate with the backend service */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Authentication")
    void Authenticate(const FString& User, const FString& Password);

    // ========================================================================
    // Payment Operations
    // ========================================================================

    /** Create a new payment */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Payments", meta = (CallInEditor = "true"))
    void CreatePayment(const FUnionPayPaymentRequest& Request);

    /** Get payment status */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Payments")
    void GetPaymentStatus(const FString& PaymentId);

    /** Confirm a payment (for two-step payments) */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Payments")
    void ConfirmPayment(const FString& PaymentId);

    /** Cancel a payment */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Payments")
    void CancelPayment(const FString& PaymentId, const FString& Reason = TEXT("User cancellation"));

    /** Refund a payment */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Payments")
    void RefundPayment(const FString& PaymentId, const FUnionPayRefundRequest& RefundRequest);

    /** Get payment by ID */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Payments")
    void GetPayment(const FString& PaymentId);

    // ========================================================================
    // Transaction History
    // ========================================================================

    /** Get transaction history for a player */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Transactions")
    void GetTransactionHistory(const FString& PlayerId, int32 Limit = 50, int32 Offset = 0);

    /** Get specific transaction */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Transactions")
    void GetTransaction(const FString& TransactionId);

    // ========================================================================
    // Payment Methods Management
    // ========================================================================

    /** Get saved payment methods for a player */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|PaymentMethods")
    void GetPaymentMethods(const FString& PlayerId);

    /** Add a new payment method */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|PaymentMethods")
    void AddPaymentMethod(const FString& PlayerId, const FUnionPayPaymentMethodRequest& MethodRequest);

    /** Remove a payment method */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|PaymentMethods")
    void RemovePaymentMethod(const FString& PaymentMethodId);

    /** Set default payment method */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|PaymentMethods")
    void SetDefaultPaymentMethod(const FString& PaymentMethodId);

    // ========================================================================
    // Utility Functions
    // ========================================================================

    /** Validate payment request */
    UFUNCTION(BlueprintPure, Category = "UnionPay|Validation")
    bool ValidatePaymentRequest(const FUnionPayPaymentRequest& Request, FUnionPayError& OutError) const;

    /** Check if currency is supported */
    UFUNCTION(BlueprintPure, Category = "UnionPay|Validation")
    bool IsCurrencySupported(const FString& Currency) const;

    /** Check if payment method is supported */
    UFUNCTION(BlueprintPure, Category = "UnionPay|Validation")
    bool IsPaymentMethodSupported(EUnionPayPaymentMethod PaymentMethod) const;

    /** Get supported currencies */
    UFUNCTION(BlueprintPure, Category = "UnionPay|Info")
    TArray<FString> GetSupportedCurrencies() const;

    /** Get supported payment methods */
    UFUNCTION(BlueprintPure, Category = "UnionPay|Info")
    TArray<EUnionPayPaymentMethod> GetSupportedPaymentMethods() const;

    /** Get minimum payment amount for currency */
    UFUNCTION(BlueprintPure, Category = "UnionPay|Info")
    double GetMinimumPaymentAmount(const FString& Currency) const;

    /** Get maximum payment amount for currency */
    UFUNCTION(BlueprintPure, Category = "UnionPay|Info")
    double GetMaximumPaymentAmount(const FString& Currency) const;

    // ========================================================================
    // Webhooks & Callbacks
    // ========================================================================

    /** Process webhook notification */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Webhooks")
    bool ProcessWebhookNotification(const FString& WebhookPayload, const FString& Signature);

    /** Verify webhook signature */
    UFUNCTION(BlueprintPure, Category = "UnionPay|Webhooks")
    bool VerifyWebhookSignature(const FString& Payload, const FString& Signature, const FString& Secret) const;

    // ========================================================================
    // Delegates (Blueprint Events)
    // ========================================================================

    /** Called when a payment is completed successfully */
    UPROPERTY(BlueprintAssignable, Category = "UnionPay|Events")
    FOnPaymentCompleted OnPaymentCompleted;

    /** Called when authentication is successful */
    UPROPERTY(BlueprintAssignable, Category = "UnionPay|Events")
    FOnAuthenticationSuccess OnAuthenticationSuccess;

    /** Called when authentication fails */
    UPROPERTY(BlueprintAssignable, Category = "UnionPay|Events")
    FOnAuthenticationFailure OnAuthenticationFailure;

    /** Called when a payment fails */
    UPROPERTY(BlueprintAssignable, Category = "UnionPay|Events")
    FOnPaymentFailed OnPaymentFailed;

    /** Called when a payment is cancelled */
    UPROPERTY(BlueprintAssignable, Category = "UnionPay|Events")
    FOnPaymentCancelled OnPaymentCancelled;

    /** Called when payment status changes */
    UPROPERTY(BlueprintAssignable, Category = "UnionPay|Events")
    FOnPaymentStatusChanged OnPaymentStatusChanged;

    /** Called when a refund is completed */
    UPROPERTY(BlueprintAssignable, Category = "UnionPay|Events")
    FOnRefundCompleted OnRefundCompleted;

    // ========================================================================
    // Advanced Features
    // ========================================================================

    /** Enable/disable sandbox mode */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Advanced")
    void SetSandboxMode(bool bEnabled);

    /** Get current sandbox mode status */
    UFUNCTION(BlueprintPure, Category = "UnionPay|Advanced")
    bool IsSandboxMode() const;

    /** Set custom user agent */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Advanced")
    void SetUserAgent(const FString& UserAgent);

    /** Set request timeout */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Advanced")
    void SetRequestTimeout(float TimeoutSeconds);

    /** Enable/disable request logging */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Advanced")
    void SetLoggingEnabled(bool bEnabled);

    /** Clear cached data */
    UFUNCTION(BlueprintCallable, Category = "UnionPay|Advanced")
    void ClearCache();

    /** Get plugin statistics */
    UFUNCTION(BlueprintPure, Category = "UnionPay|Advanced")
    FUnionPayStatistics GetStatistics() const;

private:
    /** Configuration */
    UPROPERTY()
    FUnionPayConfig Configuration;

    /** Security manager */
    UPROPERTY()
    TObjectPtr<UUnionPaySecurityManager> SecurityManager;

    /** Network manager */
    TSharedPtr<FUnionPayNetworkManager> NetworkManager;

    /** Cache manager */
    TSharedPtr<FUnionPayCacheManager> CacheManager;

    /** Initialization state */
    bool bIsInitialized;

    /** Active payment requests */
    TMap<FString, FUnionPayPaymentRequest> ActivePayments;

    /** Request statistics */
    mutable FUnionPayStatistics Statistics;

    // ========================================================================
    // Internal Helper Functions
    // ========================================================================

    /** Generate unique request ID */
    FString GenerateRequestId() const;

    /** Build API endpoint URL */
    FString BuildApiUrl(const FString& Endpoint) const;

    /** Create common HTTP headers */
    TMap<FString, FString> CreateHttpHeaders(const FString& RequestId = TEXT("")) const;

    /** Handle HTTP response */
    void HandleHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, const FString& RequestType, const FString& PaymentId = TEXT(""));

    /** Parse payment response */
    bool ParsePaymentResponse(const FString& ResponseBody, FUnionPayPaymentResponse& OutResponse, FUnionPayError& OutError) const;

    /** Parse error response */
    FUnionPayError ParseErrorResponse(const FString& ResponseBody) const;

    /** Validate API response */
    bool ValidateApiResponse(FHttpResponsePtr Response, FUnionPayError& OutError) const;

    /** Cache payment data */
    void CachePaymentData(const FString& PaymentId, const FUnionPayPaymentResponse& Response);

    /** Get cached payment data */
    bool GetCachedPaymentData(const FString& PaymentId, FUnionPayPaymentResponse& OutResponse) const;

    /** Update payment statistics */
    void UpdateStatistics(const FString& Operation, bool bSuccess, float ResponseTime) const;

    /** Log request/response for debugging */
    void LogRequest(const FString& Method, const FString& Url, const FString& Body) const;
    void LogResponse(int32 StatusCode, const FString& Body) const;

    // ========================================================================
    // Callback Handlers
    // ========================================================================

    /** Handle create payment response */
    void HandleCreatePaymentResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    /** Handle payment status response */
    void HandlePaymentStatusResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    /** Handle refund response */
    void HandleRefundResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    /** Handle transaction history response */
    void HandleTransactionHistoryResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    /** Handle payment methods response */
    void HandlePaymentMethodsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};