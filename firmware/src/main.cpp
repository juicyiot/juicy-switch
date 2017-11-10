#include <Arduino.h>
#include "WiFi/WiFiConnection.h"

WiFiConnection connection;

void setup() {
    Serial.begin(115200);
	delay(5000);

	if (connection.connect(false)) {
		Serial.println("Connected.");
	}
}

void loop() {

}
