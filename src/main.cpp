#include <Arduino.h>

#include "drivers/gps/gps.h"
#include "drivers/buzzer/buzzer.h"
#include "geofences/geofence.h"
#include "communications/api/api.h"

constexpr unsigned long LOCATION_INTERVAL = 30000;
constexpr unsigned long STATUS_INTERVAL = 1000;

unsigned long lastLocationSend = 0;
unsigned long lastStatusPrint = 0;
bool firstLocationSend = true;

void setup() {
    Serial.begin(115200);

    buzzerBegin();
    gpsHasBegin();
    geofenceBegin();
    apiBegin();

    Serial.println("HelmGuard started");
    Serial.println("Waiting for GPS fix...");
}

void loop() {
    gpsHasUpdate();

    bool gpsFix = gpsHasFix();
    bool outsideGeofence = false;

    if (gpsFix) {
        outsideGeofence = geofenceUpdate(
            gpsGetLatitude(),
            gpsGetLongitude()
        );
    }

    buzzerUpdate(gpsFix, outsideGeofence);

    unsigned long now = millis();

    if (now - lastStatusPrint >= STATUS_INTERVAL) {
        lastStatusPrint = now;

        if (!gpsFix) {
            Serial.println("GPS: NO FIX | Buzzer: INTERMITTENT");
        } else {
            Serial.println("=============================");
            Serial.print("Latitude: ");
            Serial.println(gpsGetLatitude(), 6);

            Serial.print("Longitude: ");
            Serial.println(gpsGetLongitude(), 6);

            Serial.print("Satellites: ");
            Serial.println(gpsGetSatellites());

            Serial.print("Distance: ");
            Serial.print(geofenceGetDistanceMeters(), 2);
            Serial.println(" m");

            Serial.print("Geofence: ");
            Serial.println(
                outsideGeofence ? "OUTSIDE" : "INSIDE"
            );

            Serial.print("Buzzer: ");
            Serial.println(
                outsideGeofence ? "CONTINUOUS" : "OFF"
            );
        }
    }

    if (gpsFix &&
        (firstLocationSend ||
         now - lastLocationSend >= LOCATION_INTERVAL)) {

        firstLocationSend = false;
        lastLocationSend = now;

        if (apiSendLocation()) {
            Serial.println("Location sent successfully");
        } else {
            Serial.println("Failed to send location");
        }
    }

    delay(2);
}
