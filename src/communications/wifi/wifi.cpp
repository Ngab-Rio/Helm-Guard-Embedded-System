#include "wifi.h"

#include <WiFi.h>

#include "config/app_config.h"

namespace {
    const char* WIFI_SSID = HELM_GUARD_WIFI_SSID;
    const char* WIFI_PASSWORD = HELM_GUARD_WIFI_PASSWORD;
}

bool wifiBegin()
{
    if (WIFI_SSID[0] == '\0') {
        Serial.println("Wi-Fi is not configured");
        return false;
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to Wi-Fi");
    const unsigned long startedAt = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - startedAt < HELM_GUARD_WIFI_TIMEOUT_MS) {
        delay(500);
        Serial.print(".");
    }

    if (!wifiIsConnected()) {
        Serial.println("\nWi-Fi connection timed out");
        return false;
    }

    Serial.println("\nWi-Fi connected");
    Serial.println(WiFi.localIP());
    return true;
}

bool wifiIsConnected()
{
    return WiFi.status() == WL_CONNECTED;
}