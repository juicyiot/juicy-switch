#include <Arduino.h>
#include "ConfigServer/ConfigServer.h"

ConfigServer config("juciy_config", "password");

void setup() {
    Serial.begin(115200);
	delay(5000);

	config.setup();
}

void loop() {
	if (!config.done) { config.run(); }
}
