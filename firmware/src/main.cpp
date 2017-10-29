#include <Arduino.h>
#include "WiFi/Connection.h"
#include "Config/ConfigServer.h"
#include "API/API.h"

API api;

bool apiIsSetup = false;

void setup() {
    Serial.begin(115200);
	delay(5000);

	Connection connection;
	connection.connect();
	api.setup();
}

void loop() {
	api.serve();
}
