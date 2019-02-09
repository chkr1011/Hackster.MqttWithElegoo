EthernetClient _ethernetClient;
PubSubClient _mqttClient;

// Replace this IP with the IP of your broker!
const char _brokerIP[] = "192.168.1.181";

// These are the MQTT callbacks which are allowing
// to "attach" to MQTT message handling.
// The method signature is (String topic, String payload).
// This  can be optimized by adding a bool as return value
// which will break the loop once the message is processed.
typedef void (*mqttMessageCallback)(String, String);

mqttMessageCallback _mqttMessageCallbacks[10];
uint8_t _mqttMessageCallbackIndex = 0;

uint64_t _oldMillis;

void attachMqttMessageCallback(mqttMessageCallback callback)
{
    _mqttMessageCallbacks[_mqttMessageCallbackIndex] = callback;
    _mqttMessageCallbackIndex++;
}

void setupLogging()
{
    Serial.begin(115200);
    Serial.println(F("Starting..."));
}

void setupNetwork()
{
    uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

    // Use DHCP to obtain network settings.
    Ethernet.begin(mac);
    Serial.print(F("IP = "));
    Serial.println(Ethernet.localIP());
}

void mqttCallback(char *topic, uint8_t *payload, unsigned int payloadLength)
{
    // For this tutorial and a basic understanding working with 
    // strings is way more straight forward even if it requires
    // more RAM than working with the buffer directly with memcmp etc.

    String topicString = String(topic);

    String payloadString;
    if (payloadLength > 0)
    {
        if (payload[payloadLength - 1] == '\0')
        {
            payloadString = String((char*)payload);
        }
        else
        {
            // Convert the string to a null-terminated one.
            unsigned char buffer[payloadLength + 1];
            for (uint16_t i = 0; i < payloadLength; i++)
            {
                buffer[i] = payload[i];
            }

            buffer[payloadLength] = '\0';
            payloadString = String((char *)buffer);    
        }
    }
    else
    {
        payloadString = String(F(""));
    }
    
    Serial.print(F("MQTT IN "));
    Serial.print(topicString);
    Serial.print(F(" "));
    Serial.println(payloadString);

    if (topicString.endsWith(F("/PING")))
    {
        _mqttClient.publish("awesome_device/PONG", 0);
        return;
    }

    for (uint8_t i = 0; i < _mqttMessageCallbackIndex; i++)
    {
        _mqttMessageCallbacks[i](topicString, payloadString);
    }
}

void setupMqttClient()
{
    // Link MQTT lib with network lib.
    _mqttClient.setClient(_ethernetClient);
    _mqttClient.setCallback(mqttCallback);
    _mqttClient.setServer(_brokerIP, 1883);

    bool isConnected = _mqttClient.connect("awesome_device");
    if (!isConnected)
    {
        delay(1000);

        // Try again one more time.
        isConnected = _mqttClient.connect("awesome_device");
    }

    if (isConnected)
    {
        Serial.println(F("MQTT connected"));

        // Ensure that the device will receive all messages which have the
        // device prefix. Other messages are not relevant.
        _mqttClient.subscribe("awesome_device/#");
    }
    else
    {
        Serial.println(F("MQTT failed"));
    }
}

void publishMqttMessage(const char *topic, const char *payload)
{
    Serial.print(F("MQTT OUT "));
    Serial.print(topic);
    Serial.print(F(" "));
    Serial.println(payload);

    _mqttClient.publish(topic, payload);
}

void setupShared()
{
    setupLogging();
    setupNetwork();
    setupMqttClient();
}

uint16_t loopShared()
{
    delay(10);
    
    // Calculate the time which is elapsed since the last
    // time this methid was reached.
    uint64_t newMillis = millis();
    uint64_t delay = newMillis - _oldMillis;
    _oldMillis = newMillis;

    // Maintain MQTT connection and process messages.
    _mqttClient.loop();

    return delay;
}