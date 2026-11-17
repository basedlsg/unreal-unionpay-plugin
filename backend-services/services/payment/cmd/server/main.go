package main

import (
	"context"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/signal"
	"syscall"
	"time"

	"github.com/gin-gonic/gin"
	"github.com/prometheus/client_golang/prometheus/promhttp"
	"github.com/sirupsen/logrus"
	"github.com/unionpay/payment-service/internal/config"
	"github.com/unionpay/payment-service/internal/database"
	"github.com/unionpay/payment-service/internal/handlers"
	"github.com/unionpay/payment-service/internal/middleware"
	"github.com/unionpay/payment-service/internal/redis"
	"github.com/unionpay/payment-service/internal/services"
)

func main() {
	// Load configuration
	cfg, err := config.Load()
	if err != nil {
		log.Fatal("Failed to load configuration:", err)
	}

	// Setup logger
	logger := setupLogger(cfg)

	// Initialize database
	db, err := database.Initialize(cfg.DatabaseURL)
	if err != nil {
		logger.Fatal("Failed to initialize database:", err)
	}

	// Initialize Redis
	redisClient, err := redis.Initialize(cfg.RedisURL)
	if err != nil {
		logger.Fatal("Failed to initialize Redis:", err)
	}

	// Initialize services
	paymentService := services.NewPaymentService(db, redisClient, cfg, logger)
	authService := services.NewAuthService(db, redisClient, cfg, logger)
	fraudService := services.NewFraudService(redisClient, cfg, logger)

	// Initialize handlers
	paymentHandler := handlers.NewPaymentHandler(paymentService, authService, fraudService, logger)
	healthHandler := handlers.NewHealthHandler(db, redisClient, logger)

	// Setup router
	router := setupRouter(cfg, paymentHandler, healthHandler, logger)

	// Setup server
	server := &http.Server{
		Addr:         fmt.Sprintf(":%s", cfg.Port),
		Handler:      router,
		ReadTimeout:  15 * time.Second,
		WriteTimeout: 15 * time.Second,
		IdleTimeout:  60 * time.Second,
	}

	// Start server
	go func() {
		logger.WithField("port", cfg.Port).Info("Starting payment service")
		if err := server.ListenAndServe(); err != nil && err != http.ErrServerClosed {
			logger.Fatal("Failed to start server:", err)
		}
	}()

	// Wait for interrupt signal
	quit := make(chan os.Signal, 1)
	signal.Notify(quit, syscall.SIGINT, syscall.SIGTERM)
	<-quit

	logger.Info("Shutting down payment service...")

	// Graceful shutdown with timeout
	ctx, cancel := context.WithTimeout(context.Background(), 30*time.Second)
	defer cancel()

	if err := server.Shutdown(ctx); err != nil {
		logger.Fatal("Server forced to shutdown:", err)
	}

	logger.Info("Payment service stopped")
}

func setupLogger(cfg *config.Config) *logrus.Logger {
	logger := logrus.New()

	// Set log level
	level, err := logrus.ParseLevel(cfg.LogLevel)
	if err != nil {
		level = logrus.InfoLevel
	}
	logger.SetLevel(level)

	// Set log format
	if cfg.LogFormat == "json" {
		logger.SetFormatter(&logrus.JSONFormatter{
			TimestampFormat: time.RFC3339,
		})
	} else {
		logger.SetFormatter(&logrus.TextFormatter{
			FullTimestamp: true,
		})
	}

	return logger
}

func setupRouter(cfg *config.Config, paymentHandler *handlers.PaymentHandler, healthHandler *handlers.HealthHandler, logger *logrus.Logger) *gin.Engine {
	// Set gin mode
	if cfg.Environment == "production" {
		gin.SetMode(gin.ReleaseMode)
	}

	router := gin.New()

	// Middleware
	router.Use(middleware.Logger(logger))
	router.Use(middleware.Recovery(logger))
	router.Use(middleware.CORS(cfg))
	router.Use(middleware.RequestID())

	// Health check endpoint
	router.GET("/health", healthHandler.Health)
	router.GET("/ready", healthHandler.Ready)

	// Metrics endpoint
	router.GET("/metrics", gin.WrapH(promhttp.Handler()))

	// API routes
	api := router.Group("/api/v1")
	{
		// Authentication required routes
		auth := api.Group("")
		auth.Use(middleware.Authentication(cfg.JWTSecret))
		{
			// Payment endpoints
			payments := auth.Group("/payments")
			{
				payments.POST("", paymentHandler.CreatePayment)
				payments.GET("/:id", paymentHandler.GetPayment)
				payments.POST("/:id/confirm", paymentHandler.ConfirmPayment)
				payments.POST("/:id/cancel", paymentHandler.CancelPayment)
				payments.POST("/:id/refund", paymentHandler.RefundPayment)
			}

			// Transaction history
			transactions := auth.Group("/transactions")
			{
				transactions.GET("", paymentHandler.GetTransactions)
				transactions.GET("/:id", paymentHandler.GetTransaction)
			}

			// Payment methods
			methods := auth.Group("/payment-methods")
			{
				methods.GET("", paymentHandler.GetPaymentMethods)
				methods.POST("", paymentHandler.AddPaymentMethod)
				methods.DELETE("/:id", paymentHandler.RemovePaymentMethod)
			}
		}

		// Public webhooks (no auth required)
		webhooks := api.Group("/webhooks")
		{
			webhooks.POST("/unionpay", paymentHandler.UnionPayWebhook)
			webhooks.POST("/ppro", paymentHandler.PPROWebhook)
			webhooks.POST("/mastercard", paymentHandler.MastercardWebhook)
		}
	}

	return router
}