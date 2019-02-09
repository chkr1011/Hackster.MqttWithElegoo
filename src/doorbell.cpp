#define BUZZER_PIN 9
#define AUTO_OFF_TIMEOUT 1000 // Beep for 1 second.

int32_t _doorBellDuration = AUTO_OFF_TIMEOUT;

void turnDoorbellOff()
{
    // The buzzer is also inverted like the lamp.
    // Please have a look at the lamp to find reasons
    // for this approach.
    digitalWrite(BUZZER_PIN, HIGH);

    Serial.println(F("RING end"));
}

void turnDoorbellOn()
{
    digitalWrite(BUZZER_PIN, LOW);
    _doorBellDuration = AUTO_OFF_TIMEOUT;

    Serial.println(F("RING start"));
}

void handleDoorbellMqttMessage(String topic, String payload)
{
    if (topic.endsWith(F("/doorbell/trigger")))
    {
        turnDoorbellOn();
    }
}

void setupDoorbell()
{
    pinMode(BUZZER_PIN, OUTPUT);
    turnDoorbellOff();

    attachMqttMessageCallback(handleDoorbellMqttMessage);
}

void loopDoorbell(uint16_t elapsedMillies)
{
    if (_doorBellDuration <= 0)
    {
        // The countdown is not active at the moment.
        return;
    }

    _doorBellDuration -= elapsedMillies;
    if (_doorBellDuration > 0)
    {
        // Do nothing because the timeout is not yet elapsed.
        return;
    }

    turnDoorbellOff();
}
