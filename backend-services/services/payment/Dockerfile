# Multi-stage build for Go payment service
FROM golang:1.21-alpine AS builder

# Install build dependencies
RUN apk add --no-cache git ca-certificates tzdata

# Set working directory
WORKDIR /app

# Copy go mod files
COPY go.mod go.sum ./

# Download dependencies
RUN go mod download

# Copy source code
COPY . .

# Build the application
RUN CGO_ENABLED=0 GOOS=linux go build -a -installsuffix cgo -o payment-service ./cmd/server

# Final stage
FROM alpine:3.18

# Install ca-certificates for HTTPS requests
RUN apk --no-cache add ca-certificates tzdata

# Create non-root user
RUN addgroup -g 1001 -S unionpay && \
    adduser -u 1001 -S unionpay -G unionpay

# Set working directory
WORKDIR /app

# Copy binary from builder stage
COPY --from=builder /app/payment-service .

# Copy configuration files
COPY --from=builder /app/configs ./configs

# Create logs directory
RUN mkdir -p /app/logs && \
    chown -R unionpay:unionpay /app

# Switch to non-root user
USER unionpay

# Expose port
EXPOSE 8080

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD wget --no-verbose --tries=1 --spider http://localhost:8080/health || exit 1

# Run the binary
CMD ["./payment-service"]