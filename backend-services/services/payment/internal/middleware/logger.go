package middleware

import (
	"time"

	"github.com/gin-gonic/gin"
	"github.com/sirupsen/logrus"
)

func Logger(log *logrus.Logger) gin.HandlerFunc {
	return func(c *gin.Context) {
		start := time.Now()
		path := c.Request.URL.Path
		raw := c.Request.URL.RawQuery

		c.Next()

		end := time.Now()
		latency := end.Sub(start)
		clientIP := c.ClientIP()
		method := c.Request.Method
		statusCode := c.Writer.Status()
		errorMessage := c.Errors.ByType(gin.ErrorTypePrivate).String()
		bodySize := c.Writer.Size()

		if raw != "" {
			path = path + "?" + raw
		}

		entry := log.WithFields(logrus.Fields{
			"statusCode": statusCode,
			"latency":    latency,
			"clientIP":   clientIP,
			"method":     method,
			"path":       path,
			"bodySize":   bodySize,
		})

		if errorMessage != "" {
			entry.Error(errorMessage)
		} else {
			entry.Info()
		}
	}
}