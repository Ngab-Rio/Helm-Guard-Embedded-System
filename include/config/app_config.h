#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#if __has_include("config/secrets.h")
#include "config/secrets.h"
#endif

#ifndef HELM_GUARD_WIFI_SSID
#define HELM_GUARD_WIFI_SSID ""
#endif

#ifndef HELM_GUARD_WIFI_PASSWORD
#define HELM_GUARD_WIFI_PASSWORD ""
#endif

#ifndef HELM_GUARD_API_URL
#define HELM_GUARD_API_URL "http://127.0.0.1:3000/api/locations"
#endif

#ifndef HELM_GUARD_DEVICE_ID
#define HELM_GUARD_DEVICE_ID "HG-001"
#endif

#ifndef HELM_GUARD_GEOFENCE_RADIUS_METERS
#define HELM_GUARD_GEOFENCE_RADIUS_METERS 30.0
#endif

#define HELM_GUARD_WIFI_TIMEOUT_MS 15000UL
#define HELM_GUARD_HTTP_TIMEOUT_MS 10000

#endif