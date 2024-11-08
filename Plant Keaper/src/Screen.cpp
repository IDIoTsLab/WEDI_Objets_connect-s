#include "Screen.h"

// Initialisation de `display` via la liste d'initialisation du constructeur
Screen::Screen() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

void Screen::setup() {
    // Initialisation de l'écran OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println(F("Échec de l'initialisation de l'écran OLED"));
        delay(5000);
        ESP.restart();  // Redémarrage de l'ESP en cas d'échec
    }
    display.clearDisplay();
    display.display();
}

void Screen::Clear(){
    display.clearDisplay();
}

void Screen::Display(){
    display.display(); // Mise à jour de l'affichage
}


void Screen::loop() {
    display.drawRect(0, 0, 128, 16, WHITE); // Écran du haut
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(32, 5);
    display.println(F("Plant Keeper"));
}



void Screen::write(int space, String text, float value, String unite, bool TextOrNot){
    int yPosition = 17 + (space * 8);
    display.setCursor(0, yPosition);
    display.print(text);



    if (TextOrNot == false){
        display.print(" : ");
        display.print(value);
        display.print(" ");
        display.print(unite);
    } 


}


Screen::~Screen() {
    // Destructeur, si des ressources sont à libérer
}
