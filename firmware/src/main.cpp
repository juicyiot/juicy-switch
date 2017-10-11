#include <Arduino.h>
#include "ConfigServer/ConfigServer.h"

void setup() {
    Serial.begin(115200);
	delay(5000);

	ConfigServer config("juicy_switch", "password");
	config.run();
}

void loop() {
    // put your main code here, to run repeatedly:
}
