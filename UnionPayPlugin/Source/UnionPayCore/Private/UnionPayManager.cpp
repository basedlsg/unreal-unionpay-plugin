#include "UnionPayManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonObjectConverter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UUnionPayManager::UUnionPayManager()
{
    SecurityManager = CreateDefaultSubobject<UUnionPaySecurityManager>(TEXT("SecurityManager"));
    bIsInitialized = false;
}

UUnionPayManager::~UUnionPayManager()
{
}

void UUnionPayManager::BeginDestroy()
{
    Super::BeginDestroy();
}

bool UUnionPayManager::Initialize(const FString& APIEndpoint, bool bSandboxMode)
{
    Configuration.APIEndpoint = APIEndpoint;
    Configuration.bSandboxMode = bSandboxMode;
    bIsInitialized = true;
    return true;
}

void UUnionPayManager::Shutdown()
{
    bIsInitialized = false;
}

bool UUnionPayManager::IsInitialized() const
{
    return bIsInitialized;
}

FUnionPayConfig UUnionPayManager::GetConfiguration() const
{
    return Configuration;
}

bool UUnionPayManager::UpdateConfiguration(const FUnionPayConfig& NewConfig)
{
    Configuration = NewConfig;
    return true;
}

void UUnionPayManager::Authenticate(const FString& User, const FString& Password)
{
    if (!IsInitialized())
    {
        OnAuthenticationFailure.Broadcast(TEXT("Manager not initialized."));
        return;
    }

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField(TEXT("username"), User);
    JsonObject->SetStringField(TEXT("password"), Password);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    FString Url = FString::Printf(TEXT("%s/auth/token"), *Configuration.APIEndpoint);
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(Url);
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetContentAsString(RequestBody);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UUnionPayManager::HandleAuthenticationResponse);
    HttpRequest->ProcessRequest();
}

void UUnionPayManager::HandleAuthenticationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid() && Response->GetResponseCode() == 200)
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            FString Token;
            if (JsonObject->TryGetStringField(TEXT("token"), Token))
            {
                SecurityManager->SaveToken(Token);
                OnAuthenticationSuccess.Broadcast(Token);
            }
            else
            {
                OnAuthenticationFailure.Broadcast(TEXT("Failed to find 'token' in authentication response."));
            }
        }
        else
        {
            OnAuthenticationFailure.Broadcast(TEXT("Failed to parse authentication response."));
        }
    }
    else
    {
        FString ErrorStr = TEXT("Authentication request failed.");
        if (Response.IsValid())
        {
            ErrorStr += FString::Printf(TEXT(" Code: %d, Body: %s"), Response->GetResponseCode(), *Response->GetContentAsString());
        }
        OnAuthenticationFailure.Broadcast(ErrorStr);
    }
}

void UUnionPayManager::CreatePayment(const FUnionPayPaymentRequest& Request)
{
    if (!SecurityManager)
    {
        OnPaymentFailed.Broadcast(TEXT(""), FUnionPayError{ TEXT("Configuration Error"), TEXT("SecurityManager is not available.") });
        return;
    }

    FString Token = SecurityManager->GetToken();
    if (Token.IsEmpty())
    {
        OnPaymentFailed.Broadcast(TEXT(""), FUnionPayError{ TEXT("Authentication Error"), TEXT("User is not authenticated. Please call Authenticate first.") });
        return;
    }

    FString RequestBody;
    if (!FJsonObjectConverter::UStructToJsonObjectString(Request, RequestBody))
    {
        OnPaymentFailed.Broadcast(TEXT(""), FUnionPayError{ TEXT("Serialization Error"), TEXT("Failed to serialize payment request.") });
        return;
    }

    FString Url = FString::Printf(TEXT("%s/payments"), *Configuration.APIEndpoint);
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(Url);
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + Token);
    HttpRequest->SetContentAsString(RequestBody);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UUnionPayManager::HandleCreatePaymentResponse);
    HttpRequest->ProcessRequest();
}

void UUnionPayManager::HandleCreatePaymentResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    FString PaymentId = TEXT(""); 

    if (bWasSuccessful && Response.IsValid() && (Response->GetResponseCode() == 200 || Response->GetResponseCode() == 201))
    {
        FUnionPayPaymentResponse PaymentResponse;
        if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &PaymentResponse, 0, 0))
        {
            OnPaymentCompleted.Broadcast(PaymentResponse.PaymentId, PaymentResponse);
        }
        else
        {
            OnPaymentFailed.Broadcast(PaymentId, FUnionPayError{ TEXT("Serialization Error"), TEXT("Failed to parse payment response.") });
        }
    }
    else
    {
        FString ErrorMsg = TEXT("Failed to create payment.");
        if(Response.IsValid())
        {
            ErrorMsg += FString::Printf(TEXT(" Code: %d, Body: %s"), Response->GetResponseCode(), *Response->GetContentAsString());
        }
        OnPaymentFailed.Broadcast(PaymentId, FUnionPayError{ TEXT("Network Error"), ErrorMsg });
    }
}