#include <Arduino.h>
#include "config/pins.h"
#include "buzzer.h"

const unsigned long BUZZER_INTERVAL = 500;

unsigned long lastBuzzerChange = 0;
bool buzzerState = false;

void buzzerBegin() {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
}

void buzzerUpdate(bool gpsFix, bool outsideGeofence) {
    if (!gpsFix) {
        unsigned long now = millis();

        if (now - lastBuzzerChange >= BUZZER_INTERVAL) {
            lastBuzzerChange = now;
            buzzerState = !buzzerState;

            digitalWrite(
                BUZZER_PIN,
                buzzerState ? HIGH : LOW
            );
        }

        return;
    }

    buzzerState = false;
    lastBuzzerChange = millis();

    if (outsideGeofence) {
        digitalWrite(BUZZER_PIN, HIGH);
    } else {
        digitalWrite(BUZZER_PIN, LOW);
    }
}