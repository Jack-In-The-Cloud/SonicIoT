#include <M5StickCPlus2.h>
#include <WiFi.h>
#include "MenuFunctions.h"
#include "WiFiFunctions.h"
#include "BLEFunctions.h"
#include "RFIDFunctions.h"
#include "AppConnection.h"

// Define text font and color
#define TEXT_FONT 4
#define TEXT_COLOR WHITE
#define HIGHLIGHT_COLOR RED

// Define variables in this file only
unsigned long buttonPressTime = 0;
int currentMenuIndex = 0;
int currentSubMenuIndex = 0;
bool inSubMenu = false;
int networkCount = 0;
int selectedNetwork = 0;

// WiFi Network Variables
String availableNetworks[10];
const char* mainMenuItems[] = {
  "WiFi Features",
  "BLE Features",
  "RFID Features",
  "App Connection"
};
const int mainMenuItemCount = sizeof(mainMenuItems) / sizeof(mainMenuItems[0]);
const char* wifiSubMenuItems[] = {
  "Beacon Spam",
  "Deauth Attack",
  "Evil Portals",
  "Return to home"
};
const int wifiSubMenuItemCount = sizeof(wifiSubMenuItems) / sizeof(wifiSubMenuItems[0]);
const char* bleSubMenuItems[] = {
  "Bad BLE",
  "IOS Spam",
  "Android Spam",
  "Windows Spam",
  "Return to home"
};
const int bleSubMenuItemCount = sizeof(bleSubMenuItems) / sizeof(bleSubMenuItems[0]);
const char* rfidSubMenuItems[] = {
  "Read Tag",
  "Clone Tag",
  "Erase Data",
  "Return to home"
};
const int rfidSubMenuItemCount = sizeof(rfidSubMenuItems) / sizeof(rfidSubMenuItems[0]);

void displayStartupScreen() {
  Serial.println("Displaying Startup Screen");
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(TEXT_FONT);
  M5.Lcd.setTextColor(HIGHLIGHT_COLOR);
  M5.Lcd.setCursor(30, 40);
  M5.Lcd.print("SonicIoT");
  
  // Play a startup chime (simple beep for example)
  int buzzerPin = 26;  // Adjust pin number as needed
  tone(buzzerPin, 2000); // Play tone at 2000 Hz
  delay(200); // Play for 200ms
  noTone(buzzerPin); // Stop tone
  delay(2000); // Display startup screen for 2 seconds
}

void displayHome() {
  Serial.println("Displaying Home Screen");
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(TEXT_FONT);
  M5.Lcd.setTextColor(TEXT_COLOR);
  
  // Display main menu items
  int y = 10;
  for (int i = 0; i < mainMenuItemCount; i++) {
    M5.Lcd.setCursor(10, y);
    if (i == currentMenuIndex) {
      M5.Lcd.setTextColor(HIGHLIGHT_COLOR);
    } else {
      M5.Lcd.setTextColor(TEXT_COLOR);
    }
    M5.Lcd.print(mainMenuItems[i]);
    y += 20;
  }
}

void selectMenuItem() {
  if (inSubMenu) {
    switch (currentMenuIndex) {
      case 0:
        runWiFiTask(currentSubMenuIndex);
        break;
      case 1:
        runBLETask(currentSubMenuIndex);
        break;
      case 2:
        runRFIDTask(currentSubMenuIndex);
        break;
      case 3:
        connectToWebsite();  // Connect to the website
        break;
    }
  } else {
    switch (currentMenuIndex) {
      case 0:
        currentSubMenuIndex = 0;  // Reset submenu index
        displayWiFiMenu();
        break;
      case 1:
        currentSubMenuIndex = 0;  // Reset submenu index
        displayBLEMenu();
        break;
      case 2:
        currentSubMenuIndex = 0;  // Reset submenu index
        displayRFIDMenu();
        break;
      case 3:
        scanWiFiNetworks();  // Scan for available WiFi networks
        break;
      default:
        displayHome();
        break;
    }
  }
}

void displayWiFiMenu() {
  Serial.println("Displaying WiFi Menu");
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(TEXT_FONT);
  M5.Lcd.setTextColor(TEXT_COLOR);
  inSubMenu = true;
  displaySubMenu(wifiSubMenuItems, currentSubMenuIndex, wifiSubMenuItemCount);
}
void displayBLEMenu() {
  Serial.println("Displaying BLE Menu");
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(TEXT_FONT);
  M5.Lcd.setTextColor(TEXT_COLOR);
  inSubMenu = true;
  displaySubMenu(bleSubMenuItems, currentSubMenuIndex, bleSubMenuItemCount);
}

void displayRFIDMenu() {
  Serial.println("Displaying RFID Menu");
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(TEXT_FONT);
  M5.Lcd.setTextColor(TEXT_COLOR);
  inSubMenu = true;
  displaySubMenu(rfidSubMenuItems, currentSubMenuIndex, rfidSubMenuItemCount);
}

void scanWiFiNetworks() {
  Serial.println("Scanning WiFi Networks");
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(TEXT_FONT);
  M5.Lcd.setTextColor(HIGHLIGHT_COLOR);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.print("Scanning WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int n = WiFi.scanNetworks();
  networkCount = n < 10 ? n : 10;  // Limit to 10 networks

  for (int i = 0; i < networkCount; i++) {
    availableNetworks[i] = WiFi.SSID(i);
  }

  if (networkCount == 0) {
    M5.Lcd.setCursor(10, 30);
    M5.Lcd.print("No networks found.");
    delay(2000);
    displayHome();
  } else {
    selectedNetwork = 0;
    displayAvailableNetworks();
  }
}

void displayAvailableNetworks() {
  Serial.println("Displaying Available Networks");
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(TEXT_FONT);
  M5.Lcd.setTextColor(TEXT_COLOR);

  int y = 10;
  for (int i = 0; i < networkCount; i++) {
    M5.Lcd.setCursor(10, y);
    if (i == selectedNetwork) {
      M5.Lcd.setTextColor(HIGHLIGHT_COLOR);
    } else {
      M5.Lcd.setTextColor(TEXT_COLOR);
    }
    M5.Lcd.print(availableNetworks[i]);
    y += 20;
  }

  M5.Lcd.setCursor(10, y + 20);
  M5.Lcd.setTextColor(TEXT_COLOR);
  M5.Lcd.print("Press A to select");
  M5.Lcd.setCursor(10, y + 40);
  M5.Lcd.print("Press B to return");
}

void displaySubMenu(const char* subMenuItems[], int index, int itemCount) {
  Serial.println("Displaying SubMenu");
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(TEXT_FONT);
  M5.Lcd.setTextColor(TEXT_COLOR);

  int y = 10;
  for (int i = 0; i < itemCount; i++) {
    int displayIndex = (index + i) % itemCount; // Calculate the display index for scrolling
    M5.Lcd.setCursor(10, y);
    if (i == 0) { // Highlight the first item
      M5.Lcd.setTextColor(HIGHLIGHT_COLOR);
    } else {
      M5.Lcd.setTextColor(TEXT_COLOR);
    }
    M5.Lcd.print(subMenuItems[displayIndex]);
    y += 20;
  }
}

void runWiFiTask(int index) {
  Serial.println("Running WiFi Task");
  switch (index) {
    case 0:
      beaconSpam();
      break;
    case 1:
      deauthAttack();
      break;
    case 2:
      evilPortals();
      break;
    case 3:
      inSubMenu = false;
      displayHome();
      break;
    default:
      displayWiFiMenu();
      break;
  }
}

void runBLETask(int index) {
  Serial.println("Running BLE Task");
  switch (index) {
    case 0:
      badBLE();
      break;
    case 1:
      iosSpam();
      break;
    case 2:
      androidSpam();
      break;
    case 3:
      windowsSpam();
      break;
    case 4:
      inSubMenu = false;
      displayHome();
      break;
    default:
      displayBLEMenu();
      break;
  }
}

void runRFIDTask(int index) {
  Serial.println("Running RFID Task");
  switch (index) {
    case 0:
      readTag();
      break;
    case 1:
      cloneTag();
      break;
    case 2:
      eraseData();
      break;
    case 3:
      inSubMenu = false;
      displayHome();
      break;
    default:
      displayRFIDMenu();
      break;
  }
}
