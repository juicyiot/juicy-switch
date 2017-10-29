#include <Arduino.h>
#include "WiFi/Connection.h"
#include "Config/ConfigServer.h"
#include "API/API.h"

API api;

void setup() {
    Serial.begin(115200);
	delay(5000);

	Connection connection;
	if (connection.connect()) {
		api.setup();
	}
}

void loop() {
	if (api.isSetup) {
		api.serve();
	}
}
