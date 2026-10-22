#include "gps.h"

#include <TinyGPS++.h>
#include <HardwareSerial.h>

#include "config/pins.h"

namespace {
    HardwareSerial gpsSerial(2);
    TinyGPSPlus gps;
}

void gpsHasBegin() {
    gpsSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
}

void gpsHasUpdate() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }
}

bool gpsHasFix() {
    return gps.location.isValid() && (gps.location.age() < 2000);
}

double gpsGetLatitude() {
    return gps.location.lat();
}

double gpsGetLongitude() {
    return gps.location.lng();
}

double gpsGetAltitude() {
    return gps.altitude.meters();
}

int gpsGetSatellites() {
    return gps.satellites.value();
}

double gpsGetSpeed() {
    return gps.speed.kmph();
}