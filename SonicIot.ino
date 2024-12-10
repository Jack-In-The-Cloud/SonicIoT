#include <M5StickCPlus2.h>
#include "MenuFunctions.h"
#include "WiFiFunctions.h"
#include "BLEFunctions.h"
#include "RFIDFunctions.h"
#include "AppConnection.h"

void setup() {
  Serial.begin(115200); // Initialize serial communication for debugging
  M5.begin();
  displayStartupScreen();
  displayHome();
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    buttonPressTime = millis();
    selectMenuItem();
  }

  if (M5.BtnB.wasPressed()) {
    if (inSubMenu) {
      if (currentMenuIndex == 0) {
        currentSubMenuIndex = (currentSubMenuIndex + 1) % wifiSubMenuItemCount;
        displaySubMenu(wifiSubMenuItems, currentSubMenuIndex, wifiSubMenuItemCount);
      } else if (currentMenuIndex == 1) {
        currentSubMenuIndex = (currentSubMenuIndex + 1) % bleSubMenuItemCount;
        displaySubMenu(bleSubMenuItems, currentSubMenuIndex, bleSubMenuItemCount);
      } else if (currentMenuIndex == 2) {
        currentSubMenuIndex = (currentSubMenuIndex + 1) % rfidSubMenuItemCount;
        displaySubMenu(rfidSubMenuItems, currentSubMenuIndex, rfidSubMenuItemCount);
      } else if (currentMenuIndex == 3) {
        selectedNetwork = (selectedNetwork + 1) % networkCount;
        displayAvailableNetworks();
      }
    } else {
      currentMenuIndex = (currentMenuIndex + 1) % mainMenuItemCount;
      displayHome();
    }
  }

  if (M5.BtnA.pressedFor(2000)) {
    inSubMenu = false;
    displayHome();
  }
}
