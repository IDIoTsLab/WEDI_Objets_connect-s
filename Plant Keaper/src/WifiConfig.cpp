#include "WifiConfig.h"

WifiConfig::WifiConfig(const char* ssid, const char* password)
    : ssid(ssid), password(password), localIP(192, 168, 1, 1), 
      gateway(192, 168, 1, 1), subnet(255, 255, 255, 0), server(80),
      valuesSubmitted(false) {}

void WifiConfig::begin() {
    Serial.begin(115200);
    configureWiFi();
    setupRoutes();
    server.begin();
    Serial.println("Serveur HTTP of config started");

    // Bloquer tant que les valeurs ne sont pas soumises
    while (!valuesSubmitted) {
        server.handleClient();
        delay(100); // Évite de surcharger la CPU
    }
    Serial.println("Values submitted. Proceeding with the rest of the program.");
}

void WifiConfig::loop() {
    server.handleClient();
}

void WifiConfig::configureWiFi() {
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(localIP, gateway, subnet);
    delay(100);
}

void WifiConfig::setupRoutes() {
    server.on("/", [this]() { handle_OnConnect(); });
    server.on("/submit", [this]() { handle_Submit(); });
    server.onNotFound([this]() { handle_NotFound(); });
}

void WifiConfig::handle_OnConnect() {
    server.send(200, "text/html", generateHTML());
}

void WifiConfig::handle_Submit() {
    if (server.hasArg("SSID")) {
        SSID = server.arg("SSID");
    }
    if (server.hasArg("Password")) {
        Password = server.arg("Password");
    }

    // Marquer que les valeurs ont été soumises
    if (!SSID.isEmpty() && !Password.isEmpty()) {
        valuesSubmitted = true;
    }

    Serial.println("Submitted Values:");
    Serial.println("Nom du WiFi:  " + SSID);
    Serial.println("Mot de passe:  " + Password);

    String response = "<!DOCTYPE html><html><head><title>Submission Received</title></head><body>";
    response += "<h1>Submission Successful</h1>";
    response += "<p>SSID: " + SSID + "</p>";
    response += "<p>Password: " + Password + "</p>";
    response += "<a href=\"/\">Go Back</a>";
    response += "</body></html>";

    server.send(200, "text/html", response);
}

void WifiConfig::handle_NotFound() {
    server.send(404, "text/plain", "Not found");
}

String WifiConfig::generateHTML() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<title>ESP32 Web Server</title>";
    html += "<style>body { font-family: Arial; text-align: center; margin-top: 50px; }</style>";
    html += "</head><body>";
    html += "<h1>Enter Values</h1>";
    html += "<form action=\"/submit\" method=\"POST\">";
    html += "<label for=\"SSID\">WiFi Name (SSID):</label>";
    html += "<input type=\"text\" id=\"SSID\" name=\"SSID\" required><br><br>";
    html += "<label for=\"Password\">WiFi Password:</label>";
    html += "<input type=\"text\" id=\"Password\" name=\"Password\" required><br><br>";
    html += "<button type=\"submit\">Submit</button>";
    html += "</form>";
    html += "</body></html>";
    return html;
}

String WifiConfig::GetSSID() {
    return SSID;
}

String WifiConfig::GetPassword() {
    return Password;
}

void WifiConfig::stopServer() {
    server.stop();  // Arrête le serveur HTTP
    WiFi.softAPdisconnect(true);  // Désactive le point d'accès
    Serial.println("Web server stopped.");
    delay(1000);
}