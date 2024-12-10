#ifndef MENUFUNCTIONS_H
#define MENUFUNCTIONS_H

extern const char* mainMenuItems[];
extern const char* wifiSubMenuItems[];
extern const char* bleSubMenuItems[];
extern const char* rfidSubMenuItems[];
extern const int mainMenuItemCount;
extern const int wifiSubMenuItemCount;
extern const int bleSubMenuItemCount;
extern const int rfidSubMenuItemCount;
extern int currentMenuIndex;
extern int currentSubMenuIndex;
extern bool inSubMenu;
extern unsigned long buttonPressTime; // Declare as extern
extern int networkCount;              // Declare as extern
extern int selectedNetwork;           // Declare as extern

void displayStartupScreen();
void displayHome();
void selectMenuItem();
void displayWiFiMenu();
void displayBLEMenu();
void displayRFIDMenu();
void displayAppConnectionMenu();
void displaySubMenu(const char* subMenuItems[], int index, int itemCount);
void runWiFiTask(int index);
void runBLETask(int index);
void runRFIDTask(int index);
void connectToWebsite();
void scanWiFiNetworks();
void displayAvailableNetworks();

#endif
