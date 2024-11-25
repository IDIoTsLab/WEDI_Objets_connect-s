#ifndef WIFICONFIG_H
#define WIFICONFIG_H

#include <WiFi.h>
#include <WebServer.h>

class WifiConfig {
public:
    // Constructeur
    WifiConfig(const char* ssid, const char* password);

    // Méthodes principales
    void begin();          // Initialise la configuration
    void loop();           // Gère les requêtes du serveur

    void stopServer();  // Stop server

    // Méthodes pour récupérer les valeurs soumises
    String GetSSID();
    String GetPassword();

    

private:
    // Méthodes privées
    void configureWiFi();  // Configure le point d'accès Wi-Fi
    void setupRoutes();    // Configure les routes du serveur
         // Arrête le serveur et désactive le Wi-Fi

    // Gestion des routes
    void handle_OnConnect();
    void handle_Submit();
    void handle_NotFound();

    // Génération de la page HTML
    String generateHTML();

    // Membres de classe
    const char* ssid;      // SSID pour le point d'accès
    const char* password;  // Mot de passe pour le point d'accès
    IPAddress localIP;     // Adresse IP locale
    IPAddress gateway;     // Adresse de la passerelle
    IPAddress subnet;      // Masque de sous-réseau
    WebServer server;      // Serveur HTTP

    // Variables pour stocker les valeurs soumises
    String SSID;           // Valeur soumise pour le nom du Wi-Fi
    String Password;       // Valeur soumise pour le mot de passe
    bool valuesSubmitted;  // Indique si les valeurs ont été soumises
};

#endif // WIFICONFIG_H
