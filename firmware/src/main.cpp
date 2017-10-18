#include <Arduino.h>
#include "ConfigServer/ConfigServer.h"
#include "API/API.h"

ConfigServer config("juciy_config", "password", "juicy");
API api;

bool once;

void setup() {
    Serial.begin(115200);
	delay(5000);

	config.setup();
	api.setup();

	once = false;
}

void loop() {
	if (!config.isDone()) {
		config.run();
	} else if (!once) {
		once = true;
		api.setup();
	} else {
		api.serve();
	}
}
