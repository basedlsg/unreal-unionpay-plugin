package services

import (
	"database/sql"

	"github.com/go-redis/redis/v8"
	"github.com/sirupsen/logrus"
	"github.com/unionpay-plugin/payment-service/internal/config"
)

type PaymentService struct{}
type AuthService struct{}
type FraudService struct{}

func NewPaymentService(db *sql.DB, redis *redis.Client, cfg *config.Config, logger *logrus.Logger) *PaymentService {
	return &PaymentService{}
}

func NewAuthService(db *sql.DB, redis *redis.Client, cfg *config.Config, logger *logrus.Logger) *AuthService {
	return &AuthService{}
}

func NewFraudService(redis *redis.Client, cfg *config.Config, logger *logrus.Logger) *FraudService {
	return &FraudService{}
}