#include <Arduino.h>

#include <UIPEthernet.h>  // Library for the network module.
#include <PubSubClient.h> // MQTT client library.
#include <DHT.h> // Library for accessing the DHT11.

#include "shared.cpp"

#include "button.cpp"
#include "lamp.cpp"
#include "dht.cpp"
#include "doorbell.cpp"

void setup()
{
    // Shared code
    setupShared();
    
    // Device code
    //setupButton()
    //setupLamp();
    //setupDht();
    setupDoorbell();
}

void loop()
{
    uint64_t elapsedMillis = loopShared();

    //loopButton();
    //loopLamp();
    //loopDht(elapsedMillis);
    loopDoorbell(elapsedMillis);
}