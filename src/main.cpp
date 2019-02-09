#include <Arduino.h>

#include <UIPEthernet.h>  // Library for the network module.
#include <PubSubClient.h> // MQTT client library.

#include <DHT.h> // Library for accessing the DHT11.
#include <IRremote.h> // Library for reading IR signals.

#include "shared.cpp"
#include "dht.cpp"
#include "button.cpp"
#include "lamp.cpp"
#include "doorbell.cpp"
#include "irRemote.cpp"
#include "photocell.cpp"

void setup()
{
    // Shared code
    setupShared();
    
    // Device code
    //setupButton()
    //setupLamp();
    //setupDht();
    //setupDoorbell();
    //setupIRRemote();
    setupPhotocell();
}

void loop()
{
    uint64_t elapsedMS = loopShared();

    //loopButton();
    //loopLamp();
    //loopDht(elapsedMS);
    //loopDoorbell(elapsedMS);
    //loopIRRemote();
    loopPhotocell(elapsedMS);
}