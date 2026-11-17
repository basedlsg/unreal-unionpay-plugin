package middleware

import (
	"net/http"
	"net/http/httptest"
	"testing"
	"time"

	"github.com/dgrijalva/jwt-go"
	"github.com/gin-gonic/gin"
	"github.com/stretchr/testify/assert"
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

func TestAuthenticationMiddleware(t *testing.T) {
	gin.SetMode(gin.TestMode)

	// Test case 1: Valid token
	t.Run("ValidToken", func(t *testing.T) {
		router := gin.New()
		router.Use(Authentication(testSecret))
		router.GET("/test", func(c *gin.Context) {
			c.Status(http.StatusOK)
		})

		w := httptest.NewRecorder()
		req, _ := http.NewRequest("GET", "/test", nil)
		token, _ := generateTestToken("testuser", time.Hour)
		req.Header.Set("Authorization", "Bearer "+token)

		router.ServeHTTP(w, req)

		assert.Equal(t, http.StatusOK, w.Code)
	})

	// Test case 2: Missing token
	t.Run("MissingToken", func(t *testing.T) {
		router := gin.New()
		router.Use(Authentication(testSecret))
		router.GET("/test", func(c *gin.Context) {
			c.Status(http.StatusOK)
		})

		w := httptest.NewRecorder()
		req, _ := http.NewRequest("GET", "/test", nil)

		router.ServeHTTP(w, req)

		assert.Equal(t, http.StatusUnauthorized, w.Code)
		assert.JSONEq(t, `{"error":"Authorization header required"}`, w.Body.String())
	})

	// Test case 3: Invalid token
	t.Run("InvalidToken", func(t *testing.T) {
		router := gin.New()
		router.Use(Authentication(testSecret))
		router.GET("/test", func(c *gin.Context) {
			c.Status(http.StatusOK)
		})

		w := httptest.NewRecorder()
		req, _ := http.NewRequest("GET", "/test", nil)
		req.Header.Set("Authorization", "Bearer invalid-token")

		router.ServeHTTP(w, req)

		assert.Equal(t, http.StatusUnauthorized, w.Code)
	})

	// Test case 4: Expired token
	t.Run("ExpiredToken", func(t *testing.T) {
		router := gin.New()
		router.Use(Authentication(testSecret))
		router.GET("/test", func(c *gin.Context) {
			c.Status(http.StatusOK)
		})

		w := httptest.NewRecorder()
		req, _ := http.NewRequest("GET", "/test", nil)
		token, _ := generateTestToken("testuser", -time.Hour)
		req.Header.Set("Authorization", "Bearer "+token)

		router.ServeHTTP(w, req)

		assert.Equal(t, http.StatusUnauthorized, w.Code)
		assert.JSONEq(t, `{"error":"Token is expired"}`, w.Body.String())
	})
}