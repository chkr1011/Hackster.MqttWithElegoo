#define IR_RECEIVER_PIN 4

IRrecv _irReceiver(IR_RECEIVER_PIN);
decode_results _decodeResults;

void setupIRRemote()
{
    _irReceiver.enableIRIn();
}

void loopIRRemote()
{
    bool codeReceived = _irReceiver.decode(&_decodeResults);
    if (!codeReceived)
    {
        return;
    }

    // Print the received code to the console in HEX format.
    Serial.print(F("IR IN 0x"));
    Serial.println(_decodeResults.value, HEX);

    // Forward the received code to MQTT.
    // The actual code firstly converted into a string. So 234 for example becomes "234".
    // This makes parsing and debugging in MQTTBox easier. But this approach has some overhead
    // in memory usage. The actual value is stored in HEX format.
    publishMqttMessage("awesome_device/ir/value", String(_decodeResults.value, HEX).c_str());

    // The IR receiver must be resumed after
    // each requested code.
    _irReceiver.resume();
}