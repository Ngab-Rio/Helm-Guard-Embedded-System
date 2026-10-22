#pragma once

void geofenceBegin();

bool geofenceUpdate(double latitude, double longitude);

bool geofenceIsInitialized();
double geofenceGetDistanceMeters();
