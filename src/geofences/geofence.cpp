
#include <Arduino.h>
#include <math.h>

#include "config/app_config.h"
#include "geofence.h"

namespace {
    constexpr double EARTH_RADIUS_METERS = 6371000.0;
    constexpr double DEGREES_TO_RADIANS = PI / 180.0;

    bool initialized = false;

    double centerLatitude = 0.0;
    double centerLongitude = 0.0;
    double currentDistance = 0.0;

    double calculateDistance(
        double lat1,
        double lon1,
        double lat2,
        double lon2
    ) {
        double dLat =
            (lat2 - lat1) * DEGREES_TO_RADIANS;

        double dLon =
            (lon2 - lon1) * DEGREES_TO_RADIANS;

        double a =
            sin(dLat / 2.0) * sin(dLat / 2.0) +
            cos(lat1 * DEGREES_TO_RADIANS) *
            cos(lat2 * DEGREES_TO_RADIANS) *
            sin(dLon / 2.0) * sin(dLon / 2.0);

        a = constrain(a, 0.0, 1.0);

        double c = 2.0 * atan2(
            sqrt(a),
            sqrt(1.0 - a)
        );

        return EARTH_RADIUS_METERS * c;
    }
}

void geofenceBegin() {
    initialized = false;
    centerLatitude = 0.0;
    centerLongitude = 0.0;
    currentDistance = 0.0;
}

bool geofenceUpdate(double latitude, double longitude) {
    if (!initialized) {
        centerLatitude = latitude;
        centerLongitude = longitude;
        currentDistance = 0.0;
        initialized = true;

        Serial.println("Geofence center initialized");

        Serial.print("Center latitude: ");
        Serial.println(centerLatitude, 6);

        Serial.print("Center longitude: ");
        Serial.println(centerLongitude, 6);

        return false;
    }

    currentDistance = calculateDistance(
        centerLatitude,
        centerLongitude,
        latitude,
        longitude
    );

    return currentDistance >
           HELM_GUARD_GEOFENCE_RADIUS_METERS;
}

bool geofenceIsInitialized() {
    return initialized;
}

double geofenceGetDistanceMeters() {
    return currentDistance;
}
