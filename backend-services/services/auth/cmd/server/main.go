package main

import (
	"fmt"
	"log"
	"os"
	"time"

	"github.com/dgrijalva/jwt-go"
	"github.com/gofiber/fiber/v2"
	"github.com/joho/godotenv"
)

func main() {
	// Load .env file
	err := godotenv.Load()
	if err != nil {
		log.Println("No .env file found, using environment variables")
	}

	app := fiber.New()

	app.Get("/health", func(c *fiber.Ctx) error {
		return c.SendString("Auth service is healthy")
	})

	app.Post("/login", login)

	port := os.Getenv("PORT")
	if port == "" {
		port = "8081"
	}

	log.Fatal(app.Listen(fmt.Sprintf(":%s", port)))
}

// login handler to generate a placeholder JWT
func login(c *fiber.Ctx) error {
	// In a real application, you would authenticate the user here
	// For this example, we'll just generate a token for a dummy user

	token := jwt.New(jwt.SigningMethodHS256)

	claims := token.Claims.(jwt.MapClaims)
	claims["username"] = "testuser"
	claims["user_id"] = "12345"
	claims["exp"] = time.Now().Add(time.Hour * 72).Unix()

	jwtSecret := os.Getenv("JWT_SECRET")
	if jwtSecret == "" {
		return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{
			"error": "JWT_SECRET not configured",
		})
	}

	t, err := token.SignedString([]byte(jwtSecret))
	if err != nil {
		return c.Status(fiber.StatusInternalServerError).JSON(fiber.Map{
			"error": "could not sign token",
		})
	}

	return c.JSON(fiber.Map{"token": t})
}