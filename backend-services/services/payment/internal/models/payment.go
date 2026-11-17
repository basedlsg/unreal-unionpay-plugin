package models

import (
	"time"

	"github.com/google/uuid"
	"gorm.io/gorm"
)

// PaymentStatus represents the status of a payment
type PaymentStatus string

const (
	PaymentStatusPending   PaymentStatus = "pending"
	PaymentStatusProcessing PaymentStatus = "processing"
	PaymentStatusCompleted PaymentStatus = "completed"
	PaymentStatusFailed    PaymentStatus = "failed"
	PaymentStatusCancelled PaymentStatus = "cancelled"
	PaymentStatusRefunded  PaymentStatus = "refunded"
)

// PaymentMethod represents the payment method used
type PaymentMethod string

const (
	PaymentMethodUnionPay   PaymentMethod = "unionpay"
	PaymentMethodVisa       PaymentMethod = "visa"
	PaymentMethodMastercard PaymentMethod = "mastercard"
	PaymentMethodAmex       PaymentMethod = "amex"
)

// PaymentProvider represents the payment provider
type PaymentProvider string

const (
	PaymentProviderUnionPay   PaymentProvider = "unionpay"
	PaymentProviderPPRO       PaymentProvider = "ppro"
	PaymentProviderMastercard PaymentProvider = "mastercard"
)

// Payment represents a payment transaction
type Payment struct {
	ID                     string                 `json:"id" gorm:"type:uuid;primary_key;default:gen_random_uuid()"`
	GameID                 string                 `json:"game_id" gorm:"type:varchar(255);not null;index"`
	PlayerID               string                 `json:"player_id" gorm:"type:varchar(255);not null;index"`
	Amount                 float64                `json:"amount" gorm:"type:decimal(10,2);not null"`
	Currency               string                 `json:"currency" gorm:"type:varchar(3);not null"`
	PaymentMethod          PaymentMethod          `json:"payment_method" gorm:"type:varchar(50);not null"`
	PaymentProvider        PaymentProvider        `json:"payment_provider" gorm:"type:varchar(50);not null"`
	Status                 PaymentStatus          `json:"status" gorm:"type:varchar(20);not null;default:'pending';index"`
	ExternalTransactionID  string                 `json:"external_transaction_id" gorm:"type:varchar(255);index"`
	PaymentURL             string                 `json:"payment_url,omitempty" gorm:"type:text"`
	CallbackURL            string                 `json:"callback_url" gorm:"type:text;not null"`
	RiskScore              *float64               `json:"risk_score,omitempty" gorm:"type:decimal(3,2)"`
	FraudAnalysis          *FraudAnalysis         `json:"fraud_analysis,omitempty" gorm:"type:jsonb"`
	Metadata               map[string]interface{} `json:"metadata,omitempty" gorm:"type:jsonb"`
	ErrorCode              string                 `json:"error_code,omitempty" gorm:"type:varchar(100)"`
	ErrorMessage           string                 `json:"error_message,omitempty" gorm:"type:text"`
	ProcessedAt            *time.Time             `json:"processed_at,omitempty"`
	ExpiresAt              *time.Time             `json:"expires_at,omitempty"`
	CreatedAt              time.Time              `json:"created_at" gorm:"autoCreateTime"`
	UpdatedAt              time.Time              `json:"updated_at" gorm:"autoUpdateTime"`
}

// FraudAnalysis represents fraud detection analysis results
type FraudAnalysis struct {
	RiskLevel        string  `json:"risk_level"`
	Score            float64 `json:"score"`
	Action           string  `json:"action"`
	Reasons          []string `json:"reasons,omitempty"`
	GeographicalRisk float64 `json:"geographical_risk"`
	DeviceRisk       float64 `json:"device_risk"`
	BehavioralRisk   float64 `json:"behavioral_risk"`
	AnalyzedAt       time.Time `json:"analyzed_at"`
}

// BeforeCreate hook to set ID if not provided
func (p *Payment) BeforeCreate(tx *gorm.DB) error {
	if p.ID == "" {
		p.ID = uuid.New().String()
	}
	return nil
}

// IsExpired checks if the payment has expired
func (p *Payment) IsExpired() bool {
	if p.ExpiresAt == nil {
		return false
	}
	return time.Now().After(*p.ExpiresAt)
}

// CanBeRefunded checks if the payment can be refunded
func (p *Payment) CanBeRefunded() bool {
	return p.Status == PaymentStatusCompleted
}

// CanBeCancelled checks if the payment can be cancelled
func (p *Payment) CanBeCancelled() bool {
	return p.Status == PaymentStatusPending || p.Status == PaymentStatusProcessing
}

// PaymentMethod model for storing user payment methods
type StoredPaymentMethod struct {
	ID              string                 `json:"id" gorm:"type:uuid;primary_key;default:gen_random_uuid()"`
	PlayerID        string                 `json:"player_id" gorm:"type:varchar(255);not null;index"`
	MethodType      PaymentMethod          `json:"method_type" gorm:"type:varchar(50);not null"`
	EncryptedDetails string                `json:"-" gorm:"type:text;not null"` // Never expose in JSON
	MaskedDetails   map[string]interface{} `json:"masked_details" gorm:"type:jsonb"`
	IsDefault       bool                   `json:"is_default" gorm:"default:false"`
	ExpiresAt       *time.Time             `json:"expires_at,omitempty"`
	CreatedAt       time.Time              `json:"created_at" gorm:"autoCreateTime"`
	UpdatedAt       time.Time              `json:"updated_at" gorm:"autoUpdateTime"`
}

// Transaction represents a transaction record for audit purposes
type Transaction struct {
	ID              string                 `json:"id" gorm:"type:uuid;primary_key;default:gen_random_uuid()"`
	PaymentID       string                 `json:"payment_id" gorm:"type:uuid;not null;index"`
	Type            TransactionType        `json:"type" gorm:"type:varchar(20);not null"`
	Amount          float64                `json:"amount" gorm:"type:decimal(10,2);not null"`
	Currency        string                 `json:"currency" gorm:"type:varchar(3);not null"`
	Status          TransactionStatus      `json:"status" gorm:"type:varchar(20);not null"`
	Provider        PaymentProvider        `json:"provider" gorm:"type:varchar(50);not null"`
	ExternalID      string                 `json:"external_id" gorm:"type:varchar(255)"`
	RawResponse     map[string]interface{} `json:"raw_response,omitempty" gorm:"type:jsonb"`
	ProcessingTime  int64                  `json:"processing_time_ms" gorm:"type:bigint"` // in milliseconds
	CreatedAt       time.Time              `json:"created_at" gorm:"autoCreateTime"`

	// Relationship
	Payment Payment `json:"payment,omitempty" gorm:"foreignKey:PaymentID"`
}

// TransactionType represents the type of transaction
type TransactionType string

const (
	TransactionTypePayment TransactionType = "payment"
	TransactionTypeRefund  TransactionType = "refund"
	TransactionTypeCancel  TransactionType = "cancel"
	TransactionTypeCapture TransactionType = "capture"
)

// TransactionStatus represents the status of a transaction
type TransactionStatus string

const (
	TransactionStatusSuccess TransactionStatus = "success"
	TransactionStatusFailed  TransactionStatus = "failed"
	TransactionStatusPending TransactionStatus = "pending"
)

// PaymentRequest represents the request to create a payment
type PaymentRequest struct {
	GameID        string                 `json:"game_id" binding:"required"`
	PlayerID      string                 `json:"player_id" binding:"required"`
	Amount        float64                `json:"amount" binding:"required,gt=0"`
	Currency      string                 `json:"currency" binding:"required,len=3"`
	PaymentMethod PaymentMethod          `json:"payment_method" binding:"required"`
	CallbackURL   string                 `json:"callback_url" binding:"required,url"`
	Metadata      map[string]interface{} `json:"metadata,omitempty"`
	SaveMethod    bool                   `json:"save_method,omitempty"`
}

// PaymentResponse represents the response after creating a payment
type PaymentResponse struct {
	ID                string        `json:"id"`
	Status            PaymentStatus `json:"status"`
	PaymentURL        string        `json:"payment_url,omitempty"`
	ExpiresAt         *time.Time    `json:"expires_at,omitempty"`
	RequiresRedirect  bool          `json:"requires_redirect"`
	QRCode            string        `json:"qr_code,omitempty"`
	InstructionsHTML  string        `json:"instructions_html,omitempty"`
}

// RefundRequest represents a refund request
type RefundRequest struct {
	Amount float64 `json:"amount,omitempty"` // If not provided, full refund
	Reason string  `json:"reason" binding:"required"`
}

// WebhookPayload represents the payload structure for webhooks
type WebhookPayload struct {
	EventType     string                 `json:"event_type"`
	PaymentID     string                 `json:"payment_id"`
	TransactionID string                 `json:"transaction_id,omitempty"`
	Status        PaymentStatus          `json:"status"`
	Amount        float64                `json:"amount"`
	Currency      string                 `json:"currency"`
	Timestamp     time.Time              `json:"timestamp"`
	Provider      PaymentProvider        `json:"provider"`
	Data          map[string]interface{} `json:"data,omitempty"`
	Signature     string                 `json:"signature"`
}