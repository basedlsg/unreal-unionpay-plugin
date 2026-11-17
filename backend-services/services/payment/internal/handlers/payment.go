package handlers

import (
	"database/sql"

	"github.com/gin-gonic/gin"
	"github.com/go-redis/redis/v8"
	"github.com/sirupsen/logrus"
	"github.com/unionpay-plugin/payment-service/internal/services"
)

type PaymentHandler struct{}
type HealthHandler struct{}

func NewPaymentHandler(paymentService *services.PaymentService, authService *services.AuthService, fraudService *services.FraudService, logger *logrus.Logger) *PaymentHandler {
	return &PaymentHandler{}
}

func NewHealthHandler(db *sql.DB, redis *redis.Client, logger *logrus.Logger) *HealthHandler {
	return &HealthHandler{}
}

func (h *PaymentHandler) CreatePayment(c *gin.Context)    {}
func (h *PaymentHandler) GetPayment(c *gin.Context)       {}
func (h *PaymentHandler) ConfirmPayment(c *gin.Context)   {}
func (h *PaymentHandler) CancelPayment(c *gin.Context)    {}
func (h *PaymentHandler) RefundPayment(c *gin.Context)    {}
func (h *PaymentHandler) GetTransactions(c *gin.Context)  {}
func (h *PaymentHandler) GetTransaction(c *gin.Context)   {}
func (h *PaymentHandler) GetPaymentMethods(c *gin.Context) {}
func (h *PaymentHandler) AddPaymentMethod(c *gin.Context)  {}
func (h *PaymentHandler) RemovePaymentMethod(c *gin.Context) {}
func (h *PaymentHandler) UnionPayWebhook(c *gin.Context)  {}
func (h *PaymentHandler) PPROWebhook(c *gin.Context)      {}
func (h *PaymentHandler) MastercardWebhook(c *gin.Context) {}
func (h *HealthHandler) Health(c *gin.Context)           {}
func (h *HealthHandler) Ready(c *gin.Context)            {}