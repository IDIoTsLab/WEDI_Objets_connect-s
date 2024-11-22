#include "WebServerESP.h"
#include "sensor.h"

Sensor SensorManager;

void WebServerESP::setup() {
    // Connexion au Wi-Fi
    Serial.print("Connexion à ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connecté !");
    Serial.print("Adresse IP : ");
    //String AdresseIP = toString(WiFi.localIP());
    Serial.println(WiFi.localIP());

    // Démarrer le serveur
    server.begin();
   //return AdresseIP;

}
String WebServerESP::GetAddress() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Adresse IP dans le GET :  ");
        Serial.println(WiFi.localIP());
        return "http://" + WiFi.localIP().toString(); // Convertit l'adresse IP en une URL complète
    } else {
        return "http://0.0.0.0"; // Retourne une adresse par défaut si le Wi-Fi n'est pas connecté
    }
}

void WebServerESP::loop() {
    // Vérifie si un nouveau client est disponible
    if (!client || !client.connected()) {
        client = server.available(); // Accepte un nouveau client
        if (client) {
            Serial.println("Nouveau client connecté.");
            currentLine = "";
            header = "";
            previousTime = millis(); // Démarrage du timeout
            isNewClient = true;      // Nouveau client détecté
        }
    }

    // Si un client est connecté, traiter les données disponibles
    if (client && client.connected()) {
        // Lire les données du client
        while (client.available()) {
            char c = client.read();
            Serial.write(c); // Affiche les données reçues dans la console
            header += c;

            // Si une ligne est terminée
            if (c == '\n') {
                if (currentLine.length() == 0) {
                    if (isNewClient) {
                        // Envoyer la réponse HTTP pour le nouveau client
                        String purcentMoisture = String(SensorManager.ReadMoisture());
                        String humidite = String(SensorManager.ReadHumidity());
                        String purcentLuminosity = String(SensorManager.ReadLuminosity());
                        String temperature = String(SensorManager.ReadTemperature());
                        String WaterLevelString;
                        if (SensorManager.ReadWaterlevel() == true){
                            WaterLevelString = "Niveau d'eau bas";
                            } else {
                            WaterLevelString = "Niveau d'eau OK";
                        }

                        sendResponse(purcentLuminosity,humidite,purcentMoisture,temperature,WaterLevelString);
                        isNewClient = false; // Réponse envoyée une seule fois
                    }
                } else {
                    currentLine = "";
                }
            } else if (c != '\r') {
                currentLine += c;
            }
        }

        // Si une requête est détectée, la traiter
        if (header.indexOf("GET /update?") >= 0) {
            handleClientRequest(header);
        }
    }
}



void WebServerESP::sendResponse(String luminosite, String humidite, String humiditeSol, String temperature, String waterLevel) {
    // En-tête HTTP/1.1 sans `Connection: close` pour connexion persistante
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html; charset=UTF-8");
    client.println(); // Laisser la connexion ouverte

    // Contenu HTML
    client.println("<!DOCTYPE html>");
    client.println("<html>");
    client.println("<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<style>");
    client.println("html, body { height: 100%; margin: 0; font-family: Helvetica; }");
    client.println("body { display: flex; justify-content: center; align-items: center; ");
    client.println("background-image: url('https://static.vecteezy.com/system/resources/previews/001/594/562/non_2x/organic-plant-green-background-free-vector.jpg');"); // URL externe ici
    client.println("background-size: cover; background-position: center; color: white; text-align: center; }");
    client.println(".content { background: rgba(0, 0, 0, 0.6); padding: 20px; border-radius: 10px; }");
    client.println(".button { background-color: #4CAF50; color: white; padding: 15px; margin: 5px; font-size: 20px; cursor: pointer; border: none; border-radius: 5px; }");
    client.println(".slider { width: 80%; } select { padding: 10px; font-size: 20px; }");
    client.println("</style>");
    client.println("<script>function updateValue(id, value) { document.getElementById(id).innerHTML = value; }</script>");
    client.println("</head>");
    client.println("<body>");
    client.println("<div class=\"content\">");
    client.println("<h1>Plant Keaper - Control panel</h1>");
    client.println("<form action=\"/update\" method=\"GET\">");
    client.println("<p>Humidité du sol choisie : <span id=\"humiditeValue\">0</span>%</p>");
    client.println("<select name=\"humidite\" onchange=\"updateValue('humiditeValue', this.value)\">");
    for (int i = 0; i <= 100; i += 10) {
        client.println("<option value=\"" + String(i) + "\">" + String(i) + "%</option>");
    }
    client.println("</select>");
    client.println("<p>Lumière choisie : <span id=\"lumiereValue\">500</span></p>");
    client.println("<input type=\"range\" name=\"lumiere\" min=\"500\" max=\"50000\" value=\"500\" step=\"500\" class=\"slider\" oninput=\"updateValue('lumiereValue', this.value)\">");
    client.println("<br><br><button class=\"button\" type=\"submit\">Mettre à jour</button>");
    client.println("</form>");

    // Section des statistiques de la plante
    client.println("<h2>Statistiques de la plante</h2>");
    client.println("<p>Luminosité actuelle : <span id=\"currentLuminosite\">" + String(luminosite) + "</span>%</p>");
    client.println("<p>Humidité actuelle : <span id=\"currentHumidite\">" + String(humidite) + "</span>%</p>");
    client.println("<p>Humidité du sol : <span id=\"currentHumiditeSol\">" + String(humiditeSol) + "</span>%</p>");
    client.println("<p>Température : <span id=\"currentTemperature\">" + String(temperature) + "</span> °C</p>");
    client.println("<p>Niveau d'eau : <span id=\"waterLevel\">" + String(waterLevel) + "</span></p>");

    client.println("</body></html>");
}


void WebServerESP::handleClientRequest(const String& header) {
    if (header.indexOf("GET /update?") >= 0) {
        // Extraire les valeurs de la requête
        int humiditeIndex = header.indexOf("humidite=") + 10;
        int lumiereIndex = header.indexOf("lumiere=") + 8;

        String humiditeStr = header.substring(humiditeIndex, header.indexOf("&", humiditeIndex));
        String lumiereStr = header.substring(lumiereIndex, header.indexOf(" ", lumiereIndex));

        humiditeSol = humiditeStr.toInt();
        lumiere = lumiereStr.toInt();

        Serial.println("Humidité du sol choisie : " + String(humiditeSol) + "%");
        Serial.println("Lumière choisie : " + String(lumiere));
    }
}
