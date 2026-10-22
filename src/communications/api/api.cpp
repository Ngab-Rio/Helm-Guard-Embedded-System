#include "api.h"

#include <HTTPClient.h>

#include "communications/wifi/wifi.h"
#include "config/app_config.h"
#include "communications/json/json.h"
#include "drivers/gps/gps.h"

void apiBegin()
{
    wifiBegin();
}

bool apiSendLocation()
{
    if (!wifiIsConnected()) {
        Serial.println("Wi-Fi disconnected, reconnecting");
        if (!wifiBegin()) {
            return false;
        }
    }

    if (!gpsHasFix()) {
        Serial.println("No GPS fix. Skipping POST.");
        return false;
    }

    String payload = gpsGetJson(HELM_GUARD_DEVICE_ID);

    HTTPClient http;
    http.setTimeout(HELM_GUARD_HTTP_TIMEOUT_MS);
    if (!http.begin(HELM_GUARD_API_URL)) {
        Serial.println("Could not initialize HTTP client");
        return false;
    }
    http.addHeader("Content-Type", "application/json");

    int statusCode = http.POST(payload);

    Serial.print("HTTP status: ");
    Serial.println(statusCode);

    if (statusCode > 0) {
        Serial.println(http.getString());
    } else {
        Serial.println(http.errorToString(statusCode));
    }

    http.end();

    return statusCode >= 200 && statusCode < 300;
}