#define BUTTON_PIN 3

bool _buttonWasPressed = false;

void setupButton()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loopButton()
{
    bool buttonIsPressed = digitalRead(BUTTON_PIN) == 0;
    if (_buttonWasPressed != buttonIsPressed)
    {
        if (buttonIsPressed)
        {
            Serial.println(F("Button pressed"));

            publishMqttMessage("awesome_device/button", "pressed");
        }
        else
        {
            Serial.println(F("Button released"));

            publishMqttMessage("awesome_device/button", "released");
        }
    }

    _buttonWasPressed = buttonIsPressed;
}
