#ifndef GPS_H
#define GPS_H

#include <Arduino.h>

void gpsHasBegin();
void gpsHasUpdate();
bool gpsHasFix();

double gpsGetLatitude();
double gpsGetLongitude();
double gpsGetAltitude();

int gpsGetSatellites();
double gpsGetSpeed();

#endif // GPS_H