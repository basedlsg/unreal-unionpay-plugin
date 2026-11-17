package handlers

import (
	"bytes"
	"encoding/json"
	"net/http"
	"net/http/httptest"
	"testing"
	"time"

	"github.com/dgrijalva/jwt-go"
	"github.com/gin-gonic/gin"
	"github.com/stretchr/testify/assert"
	"github.com/unionpay-plugin/payment-service/internal/middleware"
)

const testSecret = "test-secret"

func generateTestToken(username string, expiration time.Duration) (string, error) {
	claims := jwt.MapClaims{
		"username": username,
		"exp":      time.Now().Add(expiration).Unix(),
	}
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)
	return token.SignedString([]byte(testSecret))
}

func TestPaymentFlow(t *testing.T) {
	gin.SetMode(gin.TestMode)

	// Mock payment handler
	mockPaymentHandler := &PaymentHandler{}

	// Setup router
	router := gin.New()
	router.Use(middleware.Authentication(testSecret))
	router.POST("/payment", mockPaymentHandler.CreatePayment)

	// Test case: Successful payment
	t.Run("SuccessfulPayment", func(t *testing.T) {
		// Mock CreatePayment to return success
		mockPaymentHandler.CreatePayment = func(c *gin.Context) {
			c.JSON(http.StatusOK, gin.H{"status": "success"})
		}

		w := httptest.NewRecorder()
		paymentData := map[string]interface{}{"amount": 100, "currency": "USD"}
		body, _ := json.Marshal(paymentData)
		req, _ := http.NewRequest("POST", "/payment", bytes.NewBuffer(body))

		token, _ := generateTestToken("testuser", time.Hour)
		req.Header.Set("Authorization", "Bearer "+token)
		req.Header.Set("Content-Type", "application/json")

		router.ServeHTTP(w, req)

		assert.Equal(t, http.StatusOK, w.Code)
		assert.JSONEq(t, `{"status":"success"}`, w.Body.String())
	})

	// Test case: Unauthenticated payment
	t.Run("UnauthenticatedPayment", func(t *testing.T) {
		w := httptest.NewRecorder()
		paymentData := map[string]interface{}{"amount": 100, "currency": "USD"}
		body, _ := json.Marshal(paymentData)
		req, _ := http.NewRequest("POST", "/payment", bytes.NewBuffer(body))
		req.Header.Set("Content-Type", "application/json")

		router.ServeHTTP(w, req)

		assert.Equal(t, http.StatusUnauthorized, w.Code)
	})
}