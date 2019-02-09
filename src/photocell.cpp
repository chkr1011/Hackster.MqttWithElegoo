#define PHOTOCELL_PIN A0
#define MEASURE_INTERVAL 1000 // 1 second.

#define PULL_DOWN_RESISTOR_VALUE 1000

int32_t _photocellTimeout = MEASURE_INTERVAL;

void setupPhotocell()
{
    pinMode(PHOTOCELL_PIN, INPUT);
}

void loopPhotocell(uint64_t elapsedMS)
{
    _photocellTimeout -= elapsedMS;
    if (_photocellTimeout > 0)
    {
        // Do nothing because the timeout is not yet elapsed.
        return;
    }

    // Restart the timeout.
    _photocellTimeout = MEASURE_INTERVAL;

    int value = analogRead(PHOTOCELL_PIN);

    float volt = 5.0 * ((float)PULL_DOWN_RESISTOR_VALUE / (PULL_DOWN_RESISTOR_VALUE + value));

    Serial.print(F("Photocell: "));
    Serial.print(value);
    Serial.print(F(" Volt: "));
    Serial.println(volt);

    publishMqttMessage("awesome_device/photocell", String(value).c_str());
}