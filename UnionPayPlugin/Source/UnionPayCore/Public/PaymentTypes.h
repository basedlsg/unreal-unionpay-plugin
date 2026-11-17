#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PaymentTypes.generated.h"

/**
 * Payment Status Enumeration
 */
UENUM(BlueprintType, meta = (DisplayName = "UnionPay Payment Status"))
enum class EUnionPayPaymentStatus : uint8
{
    Unknown     UMETA(DisplayName = "Unknown"),
    Pending     UMETA(DisplayName = "Pending"),
    Processing  UMETA(DisplayName = "Processing"), 
    Completed   UMETA(DisplayName = "Completed"),
    Failed      UMETA(DisplayName = "Failed"),
    Cancelled   UMETA(DisplayName = "Cancelled"),
    Refunded    UMETA(DisplayName = "Refunded"),
    Expired     UMETA(DisplayName = "Expired")
};

/**
 * Payment Method Enumeration
 */
UENUM(BlueprintType, meta = (DisplayName = "UnionPay Payment Method"))
enum class EUnionPayPaymentMethod : uint8
{
    UnionPay    UMETA(DisplayName = "UnionPay"),
    Visa        UMETA(DisplayName = "Visa"),
    Mastercard  UMETA(DisplayName = "Mastercard"),
    Amex        UMETA(DisplayName = "American Express"),
    QRCode      UMETA(DisplayName = "QR Code"),
    MobileApp   UMETA(DisplayName = "Mobile App")
};

/**
 * Payment Provider Enumeration
 */
UENUM(BlueprintType, meta = (DisplayName = "UnionPay Payment Provider"))
enum class EUnionPayProvider : uint8
{
    UnionPayDirect  UMETA(DisplayName = "UnionPay Direct"),
    PPRO           UMETA(DisplayName = "PPRO"),
    Mastercard     UMETA(DisplayName = "Mastercard Gateway")
};

/**
 * Transaction Type Enumeration
 */
UENUM(BlueprintType, meta = (DisplayName = "UnionPay Transaction Type"))
enum class EUnionPayTransactionType : uint8
{
    Payment     UMETA(DisplayName = "Payment"),
    Refund      UMETA(DisplayName = "Refund"),
    Cancel      UMETA(DisplayName = "Cancel"),
    Capture     UMETA(DisplayName = "Capture")
};

/**
 * Error Code Enumeration
 */
UENUM(BlueprintType, meta = (DisplayName = "UnionPay Error Code"))
enum class EUnionPayErrorCode : uint8
{
    None                    UMETA(DisplayName = "No Error"),
    InvalidRequest          UMETA(DisplayName = "Invalid Request"),
    AuthenticationFailed    UMETA(DisplayName = "Authentication Failed"),
    InsufficientFunds       UMETA(DisplayName = "Insufficient Funds"),
    PaymentDeclined         UMETA(DisplayName = "Payment Declined"),
    NetworkError            UMETA(DisplayName = "Network Error"),
    Timeout                 UMETA(DisplayName = "Timeout"),
    InternalError           UMETA(DisplayName = "Internal Error"),
    UnsupportedCurrency     UMETA(DisplayName = "Unsupported Currency"),
    UnsupportedMethod       UMETA(DisplayName = "Unsupported Payment Method"),
    FraudDetected           UMETA(DisplayName = "Fraud Detected"),
    ExpiredCard             UMETA(DisplayName = "Expired Card"),
    InvalidAmount           UMETA(DisplayName = "Invalid Amount"),
    DuplicateTransaction    UMETA(DisplayName = "Duplicate Transaction")
};

/**
 * UnionPay Configuration Structure
 */
USTRUCT(BlueprintType, meta = (DisplayName = "UnionPay Configuration"))
struct UNIONPAYCORE_API FUnionPayConfig
{
    GENERATED_BODY()

    /** API Endpoint URL */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API", meta = (DisplayName = "API Endpoint"))
    FString APIEndpoint;

    /** API Key for authentication */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API", meta = (DisplayName = "API Key"))
    FString APIKey;

    /** Enable sandbox mode for testing */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API", meta = (DisplayName = "Sandbox Mode"))
    bool bSandboxMode = true;

    /** Request timeout in seconds */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network", meta = (DisplayName = "Request Timeout", ClampMin = "1.0", ClampMax = "300.0"))
    float RequestTimeout = 30.0f;

    /** Maximum retry attempts */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network", meta = (DisplayName = "Max Retry Attempts", ClampMin = "0", ClampMax = "10"))
    int32 MaxRetryAttempts = 3;

    /** Enable request logging */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (DisplayName = "Enable Logging"))
    bool bEnableLogging = false;

    /** Enable caching */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance", meta = (DisplayName = "Enable Caching"))
    bool bEnableCaching = true;

    /** Cache TTL in seconds */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance", meta = (DisplayName = "Cache TTL", ClampMin = "60", ClampMax = "3600"))
    int32 CacheTTL = 300;

    /** Default currency */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payments", meta = (DisplayName = "Default Currency"))
    FString DefaultCurrency = TEXT("USD");

    /** Webhook secret for signature verification */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Security", meta = (DisplayName = "Webhook Secret"))
    FString WebhookSecret;

    FUnionPayConfig()
    {
        APIEndpoint = TEXT("");
        APIKey = TEXT("");
        bSandboxMode = true;
        RequestTimeout = 30.0f;
        MaxRetryAttempts = 3;
        bEnableLogging = false;
        bEnableCaching = true;
        CacheTTL = 300;
        DefaultCurrency = TEXT("USD");
        WebhookSecret = TEXT("");
    }
};

/**
 * Payment Request Structure
 */
USTRUCT(BlueprintType, meta = (DisplayName = "UnionPay Payment Request"))
struct UNIONPAYCORE_API FUnionPayPaymentRequest
{
    GENERATED_BODY()

    /** Unique game identifier */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic", meta = (DisplayName = "Game ID"))
    FString GameID;

    /** Player/User identifier */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic", meta = (DisplayName = "Player ID"))
    FString PlayerID;

    /** Payment amount */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic", meta = (DisplayName = "Amount", ClampMin = "0.01"))
    double Amount = 0.0;

    /** Currency code (ISO 4217) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic", meta = (DisplayName = "Currency"))
    FString Currency = TEXT("USD");

    /** Payment method */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic", meta = (DisplayName = "Payment Method"))
    EUnionPayPaymentMethod PaymentMethod = EUnionPayPaymentMethod::UnionPay;

    /** Callback URL for payment notifications */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic", meta = (DisplayName = "Callback URL"))
    FString CallbackURL;

    /** Additional metadata */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced", meta = (DisplayName = "Metadata"))
    TMap<FString, FString> Metadata;

    /** Save payment method for future use */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced", meta = (DisplayName = "Save Payment Method"))
    bool bSavePaymentMethod = false;

    /** Payment description */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced", meta = (DisplayName = "Description"))
    FString Description;

    /** Order ID from the game */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced", meta = (DisplayName = "Order ID"))
    FString OrderID;

    FUnionPayPaymentRequest()
    {
        GameID = TEXT("");
        PlayerID = TEXT("");
        Amount = 0.0;
        Currency = TEXT("USD");
        PaymentMethod = EUnionPayPaymentMethod::UnionPay;
        CallbackURL = TEXT("");
        bSavePaymentMethod = false;
        Description = TEXT("");
        OrderID = TEXT("");
    }
};

/**
 * Payment Response Structure
 */
USTRUCT(BlueprintType, meta = (DisplayName = "UnionPay Payment Response"))
struct UNIONPAYCORE_API FUnionPayPaymentResponse
{
    GENERATED_BODY()

    /** Payment ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response", meta = (DisplayName = "Payment ID"))
    FString PaymentID;

    /** Payment status */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response", meta = (DisplayName = "Status"))
    EUnionPayPaymentStatus Status = EUnionPayPaymentStatus::Unknown;

    /** Payment URL for redirect */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response", meta = (DisplayName = "Payment URL"))
    FString PaymentURL;

    /** QR code data */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response", meta = (DisplayName = "QR Code"))
    FString QRCode;

    /** Payment expiration time */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response", meta = (DisplayName = "Expires At"))
    FDateTime ExpiresAt;

    /** Whether redirect is required */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response", meta = (DisplayName = "Requires Redirect"))
    bool bRequiresRedirect = false;

    /** Payment instructions HTML */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response", meta = (DisplayName = "Instructions"))
    FString InstructionsHTML;

    /** External transaction ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response", meta = (DisplayName = "External Transaction ID"))
    FString ExternalTransactionID;

    /** Provider used */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response", meta = (DisplayName = "Provider"))
    EUnionPayProvider Provider = EUnionPayProvider::PPRO;

    FUnionPayPaymentResponse()
    {
        PaymentID = TEXT("");
        Status = EUnionPayPaymentStatus::Unknown;
        PaymentURL = TEXT("");
        QRCode = TEXT("");
        ExpiresAt = FDateTime::MinValue();
        bRequiresRedirect = false;
        InstructionsHTML = TEXT("");
        ExternalTransactionID = TEXT("");
        Provider = EUnionPayProvider::PPRO;
    }
};

/**
 * Error Structure
 */
USTRUCT(BlueprintType, meta = (DisplayName = "UnionPay Error"))
struct UNIONPAYCORE_API FUnionPayError
{
    GENERATED_BODY()

    /** Error code */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Error", meta = (DisplayName = "Error Code"))
    EUnionPayErrorCode Code = EUnionPayErrorCode::None;

    /** Error message */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Error", meta = (DisplayName = "Message"))
    FString Message;

    /** Additional error details */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Error", meta = (DisplayName = "Details"))
    FString Details;

    /** HTTP status code */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Error", meta = (DisplayName = "HTTP Status"))
    int32 HTTPStatus = 0;

    /** Timestamp of the error */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Error", meta = (DisplayName = "Timestamp"))
    FDateTime Timestamp;

    FUnionPayError()
    {
        Code = EUnionPayErrorCode::None;
        Message = TEXT("");
        Details = TEXT("");
        HTTPStatus = 0;
        Timestamp = FDateTime::Now();
    }

    FUnionPayError(EUnionPayErrorCode InCode, const FString& InMessage, const FString& InDetails = TEXT(""))
        : Code(InCode), Message(InMessage), Details(InDetails), HTTPStatus(0), Timestamp(FDateTime::Now())
    {
    }
};

/**
 * Refund Request Structure
 */
USTRUCT(BlueprintType, meta = (DisplayName = "UnionPay Refund Request"))
struct UNIONPAYCORE_API FUnionPayRefundRequest
{
    GENERATED_BODY()

    /** Amount to refund (leave 0 for full refund) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Refund", meta = (DisplayName = "Amount", ClampMin = "0.0"))
    double Amount = 0.0;

    /** Reason for refund */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Refund", meta = (DisplayName = "Reason"))
    FString Reason;

    /** Additional metadata */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Refund", meta = (DisplayName = "Metadata"))
    TMap<FString, FString> Metadata;

    FUnionPayRefundRequest()
    {
        Amount = 0.0;
        Reason = TEXT("");
    }
};

/**
 * Refund Response Structure
 */
USTRUCT(BlueprintType, meta = (DisplayName = "UnionPay Refund Response"))
struct UNIONPAYCORE_API FUnionPayRefundResponse
{
    GENERATED_BODY()

    /** Refund ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Refund", meta = (DisplayName = "Refund ID"))
    FString RefundID;

    /** Original payment ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Refund", meta = (DisplayName = "Payment ID"))
    FString PaymentID;

    /** Refunded amount */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Refund", meta = (DisplayName = "Amount"))
    double Amount = 0.0;

    /** Currency */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Refund", meta = (DisplayName = "Currency"))
    FString Currency;

    /** Refund status */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Refund", meta = (DisplayName = "Status"))
    EUnionPayPaymentStatus Status = EUnionPayPaymentStatus::Unknown;

    /** Processing time */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Refund", meta = (DisplayName = "Processed At"))
    FDateTime ProcessedAt;

    FUnionPayRefundResponse()
    {
        RefundID = TEXT("");
        PaymentID = TEXT("");
        Amount = 0.0;
        Currency = TEXT("");
        Status = EUnionPayPaymentStatus::Unknown;
        ProcessedAt = FDateTime::Now();
    }
};

/**
 * Payment Method Request Structure
 */
USTRUCT(BlueprintType, meta = (DisplayName = "UnionPay Payment Method Request"))
struct UNIONPAYCORE_API FUnionPayPaymentMethodRequest
{
    GENERATED_BODY()

    /** Payment method type */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payment Method", meta = (DisplayName = "Method Type"))
    EUnionPayPaymentMethod MethodType = EUnionPayPaymentMethod::UnionPay;

    /** Encrypted card details */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payment Method", meta = (DisplayName = "Card Details"))
    TMap<FString, FString> CardDetails;

    /** Set as default method */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payment Method", meta = (DisplayName = "Set as Default"))
    bool bSetAsDefault = false;

    FUnionPayPaymentMethodRequest()
    {
        MethodType = EUnionPayPaymentMethod::UnionPay;
        bSetAsDefault = false;
    }
};

/**
 * Transaction Structure
 */
USTRUCT(BlueprintType, meta = (DisplayName = "UnionPay Transaction"))
struct UNIONPAYCORE_API FUnionPayTransaction
{
    GENERATED_BODY()

    /** Transaction ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction", meta = (DisplayName = "Transaction ID"))
    FString TransactionID;

    /** Payment ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction", meta = (DisplayName = "Payment ID"))
    FString PaymentID;

    /** Transaction type */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction", meta = (DisplayName = "Type"))
    EUnionPayTransactionType Type = EUnionPayTransactionType::Payment;

    /** Amount */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction", meta = (DisplayName = "Amount"))
    double Amount = 0.0;

    /** Currency */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction", meta = (DisplayName = "Currency"))
    FString Currency;

    /** Status */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction", meta = (DisplayName = "Status"))
    EUnionPayPaymentStatus Status = EUnionPayPaymentStatus::Unknown;

    /** Provider */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction", meta = (DisplayName = "Provider"))
    EUnionPayProvider Provider = EUnionPayProvider::PPRO;

    /** Created timestamp */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction", meta = (DisplayName = "Created At"))
    FDateTime CreatedAt;

    /** Processed timestamp */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transaction", meta = (DisplayName = "Processed At"))
    FDateTime ProcessedAt;

    FUnionPayTransaction()
    {
        TransactionID = TEXT("");
        PaymentID = TEXT("");
        Type = EUnionPayTransactionType::Payment;
        Amount = 0.0;
        Currency = TEXT("");
        Status = EUnionPayPaymentStatus::Unknown;
        Provider = EUnionPayProvider::PPRO;
        CreatedAt = FDateTime::Now();
        ProcessedAt = FDateTime::MinValue();
    }
};

/**
 * Statistics Structure
 */
USTRUCT(BlueprintType, meta = (DisplayName = "UnionPay Statistics"))
struct UNIONPAYCORE_API FUnionPayStatistics
{
    GENERATED_BODY()

    /** Total requests made */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics", meta = (DisplayName = "Total Requests"))
    int32 TotalRequests = 0;

    /** Successful requests */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics", meta = (DisplayName = "Successful Requests"))
    int32 SuccessfulRequests = 0;

    /** Failed requests */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics", meta = (DisplayName = "Failed Requests"))
    int32 FailedRequests = 0;

    /** Average response time */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics", meta = (DisplayName = "Average Response Time"))
    float AverageResponseTime = 0.0f;

    /** Total payments processed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics", meta = (DisplayName = "Total Payments"))
    int32 TotalPayments = 0;

    /** Successful payments */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics", meta = (DisplayName = "Successful Payments"))
    int32 SuccessfulPayments = 0;

    /** Total amount processed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics", meta = (DisplayName = "Total Amount"))
    double TotalAmount = 0.0;

    /** Cache hit rate */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics", meta = (DisplayName = "Cache Hit Rate"))
    float CacheHitRate = 0.0f;

    FUnionPayStatistics()
    {
        TotalRequests = 0;
        SuccessfulRequests = 0;
        FailedRequests = 0;
        AverageResponseTime = 0.0f;
        TotalPayments = 0;
        SuccessfulPayments = 0;
        TotalAmount = 0.0;
        CacheHitRate = 0.0f;
    }
};