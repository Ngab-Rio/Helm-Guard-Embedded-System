#include "json.h"

#include <ArduinoJson.h>
#include "drivers/gps/gps.h"

String gpsGetJson(const char* deviceId) {
    JsonDocument doc;

    const bool fix = gpsHasFix();
    doc["gps_fix"] = fix;
    doc["device_id"] = deviceId;

    if (fix) {
        doc["latitude"] = gpsGetLatitude();
        doc["longitude"] = gpsGetLongitude();
        doc["altitude"] = gpsGetAltitude();
        doc["satellites"] = gpsGetSatellites();
        doc["speed"] = gpsGetSpeed();
    }

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}