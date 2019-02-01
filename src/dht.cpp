#define DHT_PIN 2
#define PUSH_INTERVAL 5000 // Send every 5 seconds.

DHT _dht(DHT_PIN, DHT11);

int32_t _timeout = PUSH_INTERVAL;

void setupDht()
{
    // The DHT11 needs a 10k pull up resistor between POWER and DATA.
    // But in several cases the internal pullup might me enough.
    // If the device does not work properly the 10k resistor should be attached.
    // ATTENTION! The device will only return valid values every 2 seconds!
    
    pinMode(DHT_PIN, INPUT_PULLUP);
    _dht.begin();
}

void publishSensorValues()
{
    float t = _dht.readTemperature();
    float h = _dht.readHumidity();

    Serial.print(F("T="));
    Serial.print(t);
    Serial.print(F(" H="));
    Serial.println(h);

    publishMqttMessage("awesome_device/temperature", String(t).c_str());
    publishMqttMessage("awesome_device/humidity", String(h).c_str());
}

void loopDht(uint16_t elapsedMillies)
{
    _timeout -= elapsedMillies;
    if (_timeout > 0)
    {
        // Do nothing because the timeout is not yet elapsed.
        return;
    }

    // Restart the timeout.
    _timeout = PUSH_INTERVAL;

    publishSensorValues();
}
