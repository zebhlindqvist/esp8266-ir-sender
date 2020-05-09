#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WiFiClient.h>

const char* kSsid = "**********";
const char* kPassword = "**********";

ESP8266WebServer server(80);

const uint16_t kIrLed = 2;
IRsend irsend(kIrLed);

void handleRoot() {
  server.send(200, "text/plain", "/ir?code=hex");
}

void handleIr() {
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "code") {
      uint32_t code = strtoul(server.arg(i).c_str(), NULL, 16);
      irsend.sendNEC(code, 32);
      server.send(200);
      return;
    }
  }
  server.send(500);
}

void handleNotFound() {
  server.send(404, "text/plain", "404");
}

void setup(void) {
  irsend.begin();
  WiFi.begin(kSsid, kPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.on("/", handleRoot);
  server.on("/ir", handleIr);
  server.onNotFound(handleNotFound);

  server.begin();
}

void loop(void) {
  server.handleClient();
}
