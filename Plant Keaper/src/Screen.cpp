#include "Screen.h"
#include "qrcode.h"

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


void Screen::QRcode(const char *text) {
    // Démarrer le chronométrage
    uint32_t dt = millis();

    // Créer le QR code avec une version adaptée (par exemple, version 2)
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(2)]; // Version 2 pour une taille réduite
    qrcode_initText(&qrcode, qrcodeData, 2, 0, text);

    // Temps de génération
    dt = millis() - dt;
    Serial.print("QR Code Generation Time: ");
    Serial.println(dt);

    // Effacer l'écran pour afficher uniquement le QR code
    display.clearDisplay();

    // Taille des pixels d'un module QR code à l'écran
    const int pixelSize = 2; // Taille de chaque "case" du QR code en pixels
    const int offsetX = (SCREEN_WIDTH - (qrcode.size * pixelSize)) / 2; // Centrer horizontalement
    const int offsetY = 14; // Début de l'affichage à partir de la ligne 17

    // Dessiner le QR code module par module
    for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
            if (qrcode_getModule(&qrcode, x, y)) {
                // Dessiner un carré pour chaque module "rempli"
                int drawY = offsetY + y * pixelSize;
                if (drawY + pixelSize <= SCREEN_HEIGHT) { // Ne pas dépasser l'écran
                    display.fillRect(offsetX + x * pixelSize, drawY, pixelSize, pixelSize, WHITE);
                }
            }
        }
    }

    // Mettre à jour l'écran pour afficher le QR code
    display.display();
}




Screen::~Screen() {
    // Destructeur, si des ressources sont à libérer
}