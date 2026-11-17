package main

import (
	"bytes"
	"encoding/json"
	"net/http"
	"net/http/httptest"
	"testing"
	"time"

	"github.com/golang-jwt/jwt/v4"
	"github.com/stretchr/testify/assert"
)

func TestLoginHandler(t *testing.T) {
	// Set a secret key for testing
	jwtSecret = []byte("test-secret")

	// Create a request with valid credentials
	credentials := map[string]string{"username": "testuser", "password": "password"}
	body, _ := json.Marshal(credentials)
	req, err := http.NewRequest("POST", "/login", bytes.NewBuffer(body))
	assert.NoError(t, err)

	// Create a ResponseRecorder to record the response
	rr := httptest.NewRecorder()
	handler := http.HandlerFunc(loginHandler)

	// Serve the HTTP request
	handler.ServeHTTP(rr, req)

	// Check the status code
	assert.Equal(t, http.StatusOK, rr.Code)

	// Check the response body
	var response map[string]string
	err = json.Unmarshal(rr.Body.Bytes(), &response)
	assert.NoError(t, err)

	// Verify the token
	tokenString, ok := response["token"]
	assert.True(t, ok)

	// Parse the token
	token, err := jwt.Parse(tokenString, func(token *jwt.Token) (interface{}, error) {
		return jwtSecret, nil
	})
	assert.NoError(t, err)
	assert.True(t, token.Valid)

	// Verify claims
	claims, ok := token.Claims.(jwt.MapClaims)
	assert.True(t, ok)
	assert.Equal(t, "testuser", claims["username"])

	// Check expiration
	exp, ok := claims["exp"].(float64)
	assert.True(t, ok)
	assert.True(t, time.Unix(int64(exp), 0).After(time.Now()))
}