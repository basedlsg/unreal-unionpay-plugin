package config

import (
	"os"
	"strconv"
	"strings"
)

// Config holds all configuration for the payment service
type Config struct {
	// Server configuration
	Environment string
	Port        string
	LogLevel    string
	LogFormat   string
	DebugMode   bool

	// Database configuration
	DatabaseURL string

	// Redis configuration
	RedisURL string

	// JWT configuration
	JWTSecret     string
	JWTExpiration int

	// Encryption
	EncryptionKey string

	// Payment providers
	UnionPay   UnionPayConfig
	PPRO       PPROConfig
	Mastercard MastercardConfig

	// Security
	RateLimit RateLimitConfig
	CORS      CORSConfig

	// Fraud detection
	FraudDetection FraudDetectionConfig

	// Webhooks
	WebhookSecret string

	// Transaction limits
	MinTransactionAmount float64
	MaxTransactionAmount float64
	SupportedCurrencies  []string
	PaymentTimeout       int
}

// UnionPayConfig holds UnionPay specific configuration
type UnionPayConfig struct {
	MerchantID    string
	APIKey        string
	SandboxURL    string
	ProductionURL string
	Enabled       bool
}

// PPROConfig holds PPRO specific configuration
type PPROConfig struct {
	APIKey        string
	MerchantID    string
	SandboxURL    string
	ProductionURL string
	Enabled       bool
}

// MastercardConfig holds Mastercard Gateway configuration
type MastercardConfig struct {
	APIKey        string
	MerchantID    string
	SandboxURL    string
	ProductionURL string
	Enabled       bool
}

// RateLimitConfig holds rate limiting configuration
type RateLimitConfig struct {
	RequestsPerMinute int
	RequestsPerHour   int
	RequestsPerDay    int
}

// CORSConfig holds CORS configuration
type CORSConfig struct {
	AllowedOrigins []string
	AllowedMethods []string
	AllowedHeaders []string
}

// FraudDetectionConfig holds fraud detection configuration
type FraudDetectionConfig struct {
	Enabled         bool
	ModelPath       string
	ThresholdHigh   float64
	ThresholdMedium float64
}

// Load loads configuration from environment variables
func Load() (*Config, error) {
	config := &Config{
		Environment: getEnv("ENVIRONMENT", "development"),
		Port:        getEnv("PORT", "8080"),
		LogLevel:    getEnv("LOG_LEVEL", "info"),
		LogFormat:   getEnv("LOG_FORMAT", "json"),
		DebugMode:   getEnvAsBool("DEBUG_MODE", false),

		DatabaseURL: getEnv("DATABASE_URL", "postgresql://unionpay:unionpaypass@localhost:5432/unionpay"),
		RedisURL:    getEnv("REDIS_URL", "redis://:redispass@localhost:6379"),

		JWTSecret:     getEnv("JWT_SECRET", "your-super-secret-jwt-key"),
		JWTExpiration: getEnvAsInt("JWT_EXPIRATION", 3600),

		EncryptionKey: getEnv("ENCRYPTION_KEY", "32-character-encryption-key-here"),

		UnionPay: UnionPayConfig{
			MerchantID:    getEnv("UNIONPAY_MERCHANT_ID", ""),
			APIKey:        getEnv("UNIONPAY_API_KEY", ""),
			SandboxURL:    getEnv("UNIONPAY_SANDBOX_URL", "https://sandbox.unionpay.com/api/v1"),
			ProductionURL: getEnv("UNIONPAY_PRODUCTION_URL", "https://api.unionpay.com/api/v1"),
			Enabled:       getEnvAsBool("UNIONPAY_ENABLED", false),
		},

		PPRO: PPROConfig{
			APIKey:        getEnv("PPRO_API_KEY", ""),
			MerchantID:    getEnv("PPRO_MERCHANT_ID", ""),
			SandboxURL:    getEnv("PPRO_SANDBOX_URL", "https://sandbox.ppro.com/api/v1"),
			ProductionURL: getEnv("PPRO_PRODUCTION_URL", "https://api.ppro.com/api/v1"),
			Enabled:       getEnvAsBool("PPRO_ENABLED", true),
		},

		Mastercard: MastercardConfig{
			APIKey:        getEnv("MASTERCARD_API_KEY", ""),
			MerchantID:    getEnv("MASTERCARD_MERCHANT_ID", ""),
			SandboxURL:    getEnv("MASTERCARD_SANDBOX_URL", "https://sandbox-api.mastercard.com"),
			ProductionURL: getEnv("MASTERCARD_PRODUCTION_URL", "https://api.mastercard.com"),
			Enabled:       getEnvAsBool("MASTERCARD_ENABLED", false),
		},

		RateLimit: RateLimitConfig{
			RequestsPerMinute: getEnvAsInt("RATE_LIMIT_REQUESTS_PER_MINUTE", 100),
			RequestsPerHour:   getEnvAsInt("RATE_LIMIT_REQUESTS_PER_HOUR", 1000),
			RequestsPerDay:    getEnvAsInt("RATE_LIMIT_REQUESTS_PER_DAY", 10000),
		},

		CORS: CORSConfig{
			AllowedOrigins: strings.Split(getEnv("CORS_ALLOWED_ORIGINS", "*"), ","),
			AllowedMethods: strings.Split(getEnv("CORS_ALLOWED_METHODS", "GET,POST,PUT,DELETE,OPTIONS"), ","),
			AllowedHeaders: strings.Split(getEnv("CORS_ALLOWED_HEADERS", "Content-Type,Authorization,X-API-Key"), ","),
		},

		FraudDetection: FraudDetectionConfig{
			Enabled:         getEnvAsBool("FRAUD_DETECTION_ENABLED", true),
			ModelPath:       getEnv("FRAUD_MODEL_PATH", "/models/fraud_detection_model.h5"),
			ThresholdHigh:   getEnvAsFloat("FRAUD_THRESHOLD_HIGH", 0.8),
			ThresholdMedium: getEnvAsFloat("FRAUD_THRESHOLD_MEDIUM", 0.5),
		},

		WebhookSecret: getEnv("WEBHOOK_SECRET", "webhook-secret-change-this"),

		MinTransactionAmount: getEnvAsFloat("MIN_TRANSACTION_AMOUNT", 0.01),
		MaxTransactionAmount: getEnvAsFloat("MAX_TRANSACTION_AMOUNT", 10000.00),
		SupportedCurrencies:  strings.Split(getEnv("SUPPORTED_CURRENCIES", "USD,EUR,CNY,JPY,GBP"), ","),
		PaymentTimeout:       getEnvAsInt("PAYMENT_TIMEOUT_SECONDS", 300),
	}

	return config, nil
}

// Helper functions to get environment variables with defaults
func getEnv(key, defaultValue string) string {
	if value := os.Getenv(key); value != "" {
		return value
	}
	return defaultValue
}

func getEnvAsInt(key string, defaultValue int) int {
	if value := os.Getenv(key); value != "" {
		if intValue, err := strconv.Atoi(value); err == nil {
			return intValue
		}
	}
	return defaultValue
}

func getEnvAsFloat(key string, defaultValue float64) float64 {
	if value := os.Getenv(key); value != "" {
		if floatValue, err := strconv.ParseFloat(value, 64); err == nil {
			return floatValue
		}
	}
	return defaultValue
}

func getEnvAsBool(key string, defaultValue bool) bool {
	if value := os.Getenv(key); value != "" {
		if boolValue, err := strconv.ParseBool(value); err == nil {
			return boolValue
		}
	}
	return defaultValue
}

// IsProduction returns true if the environment is production
func (c *Config) IsProduction() bool {
	return c.Environment == "production"
}

// IsDevelopment returns true if the environment is development
func (c *Config) IsDevelopment() bool {
	return c.Environment == "development"
}

// GetPaymentProviderURL returns the appropriate URL for the payment provider
func (c *Config) GetUnionPayURL() string {
	if c.IsProduction() {
		return c.UnionPay.ProductionURL
	}
	return c.UnionPay.SandboxURL
}

func (c *Config) GetPPROURL() string {
	if c.IsProduction() {
		return c.PPRO.ProductionURL
	}
	return c.PPRO.SandboxURL
}

func (c *Config) GetMastercardURL() string {
	if c.IsProduction() {
		return c.Mastercard.ProductionURL
	}
	return c.Mastercard.SandboxURL
}