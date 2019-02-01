#define LAMP_PIN 5

bool isLampOn()
{
    // Remember that the GPIO is inverted.
    return digitalRead(LAMP_PIN) == 0;
}

void turnLampOn()
{
    // The state of the lamp is inverted.
    // This means that LOW means on and HIGH means off.
    // This is related to the electrical characteristics of the UNO.
    // It is simply able to drive more current when it is inverted.
    digitalWrite(LAMP_PIN, LOW);

    // You can try to set brightness here using a range from 0-255.
    // Try to pass the brightness value instead "on" and "off".
    // This will require a conversion from payload string to int.
    // Method for this: myString.toInt()
    //analogWrite(LAMP_PIN, 225);
}

void turnLampOff()
{
    digitalWrite(LAMP_PIN, HIGH);
}

void handleLampMqttMessage(String topic, String payload)
{
    if (!topic.endsWith(F("/lamp")))
    {
        // The message does not belong to the lamp.
        return;
    }

    if (payload.equals("on"))
    {
        turnLampOn();
    }
    else if (payload.equals("off"))
    {
        turnLampOff();
    }
}

void setupLamp()
{
    pinMode(LAMP_PIN, OUTPUT);
    turnLampOff();

    attachMqttMessageCallback(handleLampMqttMessage);
}

void loopLamp()
{

}