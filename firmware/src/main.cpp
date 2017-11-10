#include <Arduino.h>
#include "WiFi/WiFiConnection.h"
#include "API/API.h"

WiFiConnection connection("wifi_123", "wlankabel123!");
API api;

void setup() {
    Serial.begin(115200);
	delay(5000);

	if (connection.connect(false)) {
		Serial.print("Connected: ");
		Serial.println(connection.getIP());
		api.setup();
	}
}

void loop() {
	if (connection.isConnected()) {
		api.serve();
	}
}
