#include <Arduino.h>
#include "WiFi/WiFiConnection.h"
#include "Config/ConfigServer.h"
#include "API/API.h"

WiFiConnection connection("wifi_123", "wlankabel123!");
ConfigServer config("juicy_switch", "password", "juicy_switch");
API api;

void setup() {
    Serial.begin(115200);
	delay(5000);

	config.setup();
	config.runBlocking();

	api.setup();
}

void loop() {
	api.serve();
}
