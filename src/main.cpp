#include <Arduino.h>

#include "drivers/gps/gps.h"
#include "drivers/buzzer/buzzer.h"
#include "communications/wifi/wifi.h"
#include "communications/api/api.h"

const unsigned long LOCATION_INTERVAL = 30000;

unsigned long lastLocationSend = 0;
bool firstLocationSend = true;

void setup() {
    Serial.begin(115200);

    buzzerBegin();
    gpsHasBegin();
    apiBegin();

    Serial.println("Helm Guard started");
}

void loop() {
    gpsHasUpdate();

    buzzerUpdate(gpsHasFix());

    unsigned long now = millis();

    if (firstLocationSend ||
        now - lastLocationSend >= LOCATION_INTERVAL) {

        firstLocationSend = false;
        lastLocationSend = now;

        if (gpsHasFix()) {
            // Print GPS data to Serial
            Serial.print("Latitude: ");
            Serial.println(gpsGetLatitude(), 6);

            Serial.print("Longitude: ");
            Serial.println(gpsGetLongitude(), 6);

            Serial.print("Altitude: ");
            Serial.println(gpsGetAltitude(), 2);

            Serial.print("Satellites: ");
            Serial.println(gpsGetSatellites());

            Serial.print("Speed: ");
            Serial.println(gpsGetSpeed(), 2);

            // Send location to API
            if (apiSendLocation()) {
                Serial.println("Location sent successfully");
            } else {
                Serial.println("Failed to send location");
            }
        } else {
            Serial.println("No GPS fix available");
        }
    }

    delay(2);
}