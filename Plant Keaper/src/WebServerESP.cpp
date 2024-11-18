#include "WebServerESP.h"

String WebServerESP::setup() {
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
   // return AdresseIP;
}

void WebServerESP::loop() {
    WiFiClient client = server.available(); // Écouter les clients entrants

    if (client) {
        Serial.println("Nouveau client connecté.");
        currentTime = millis(); // Initialiser le temps actuel
        previousTime = currentTime;
        String currentLine = "";

        while (client.connected() && (millis() - previousTime <= timeoutTime)) {
            currentTime = millis();

            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                header += c;

                if (c == '\n') {
                    // Si ligne vide, envoyer la page HTML
                    if (currentLine.length() == 0) {
                        // En-tête HTTP
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html; charset=UTF-8");
                        client.println("Connection: close");
                        client.println();

                        // Page HTML
                        client.println("<!DOCTYPE html>");
                        client.println("<html>");
                        client.println("<head>");
                        client.println("<meta charset=\"UTF-8\">");
                        client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<style>");
                        client.println("html { font-family: Helvetica; text-align: center; }");
                        client.println(".button { background-color: #4CAF50; color: white; padding: 15px; margin: 5px; font-size: 20px; cursor: pointer; }");
                        client.println(".slider { width: 80%; } select { padding: 10px; font-size: 20px; }");
                        client.println("</style>");
                        client.println("<script>");
                        // Mise à jour dynamique des valeurs choisies
                        client.println("function updateValue(id, value) { document.getElementById(id).innerHTML = value; }");
                        client.println("</script>");
                        client.println("</head>");
                        client.println("<body>");
                        client.println("<h1>ESP32 - Contrôle des ressources</h1>");

                        // Formulaire pour humidité et lumière
                        client.println("<form action=\"/update\" method=\"GET\">");

                        // Humidité du sol (menu déroulant)
                        client.println("<p>Humidité du sol choisie : <span id=\"humiditeValue\">0</span>%</p>");
                        client.println("<select name=\"humidite\" onchange=\"updateValue('humiditeValue', this.value)\">");
                        for (int i = 0; i <= 100; i += 10) {
                            client.println("<option value=\"" + String(i) + "\">" + String(i) + "%</option>");
                        }
                        client.println("</select>");

                        // Lumière (slider)
                        client.println("<p>Lumière choisie : <span id=\"lumiereValue\">500</span></p>");
                        client.println("<input type=\"range\" name=\"lumiere\" min=\"500\" max=\"50000\" value=\"500\" step=\"500\" class=\"slider\" oninput=\"updateValue('lumiereValue', this.value)\">");

                        // Bouton "Mettre à jour"
                        client.println("<br><br><button class=\"button\" type=\"submit\">Mettre à jour</button>");
                        client.println("</form>");

                        client.println("</body>");
                        client.println("</html>");
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }
            }
        }

        // Traiter les requêtes HTTP
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

        // Nettoyer l'en-tête et fermer la connexion
        header = "";
        client.stop();
        Serial.println("Client déconnecté.");
    }
}
    