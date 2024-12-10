#include <M5StickCPlus2.h>
#include <WiFi.h>
#include "AppConnection.h"

// WiFi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

void connectToWebsite() {
    // Initialize the M5StickC object
    M5.begin();

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextFont(4);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.print("Connecting to WiFi...");

    // Connect to WiFi
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        M5.Lcd.print(".");
    }

    M5.Lcd.setCursor(10, 30);
    M5.Lcd.print("Connected to WiFi");

    // Here you can add the code to connect to your website
    // For example, you can use HTTPClient to connect and send data to your website

    // Example code for connecting to a website (HTTP GET request):
    /*
    #include <HTTPClient.h>

    HTTPClient http;
    http.begin("http://yourwebsite.com"); // Specify your URL
    int httpCode = http.GET(); // Make the request

    if (httpCode > 0) { // Check for the returning code
        String payload = http.getString();
        M5.Lcd.setCursor(10, 50);
        M5.Lcd.print("Website Response:");
        M5.Lcd.setCursor(10, 70);
        M5.Lcd.print(payload);
    } else {
        M5.Lcd.setCursor(10, 50);
        M5.Lcd.print("Connection failed");
    }

    http.end(); // Close connection
    */

    M5.Lcd.setCursor(10, 50);
    M5.Lcd.print("Connected!");
}
