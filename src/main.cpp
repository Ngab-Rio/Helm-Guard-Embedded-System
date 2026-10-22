#include <Arduino.h>

#include "drivers/gps/gps.h"
#include "communications/wifi/wifi.h"
#include "communications/api/api.h"

const unsigned long LOCATION_INTERVAL = 30000;

unsigned long lastLocationSend = 0;
bool firstLocationSend = true;

void setup()
{
    Serial.begin(115200);

    gpsHasBegin();
    apiBegin();

    Serial.println("Helm Guard started");
}

void loop()
{
    gpsHasUpdate();

    unsigned long now = millis();

    if (firstLocationSend ||
        now - lastLocationSend >= LOCATION_INTERVAL) {

        firstLocationSend = false;
        lastLocationSend = now;

        apiSendLocation();
    }

    delay(2);
}