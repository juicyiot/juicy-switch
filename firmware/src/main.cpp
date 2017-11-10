#include <Arduino.h>
#include "WiFi/WiFiConnection.h"

WiFiConnection connection("wifi_123", "wlankabel123!");

void setup() {
    Serial.begin(115200);
	delay(5000);

	if (connection.connect(false)) {
		Serial.print("Connected: ");
		Serial.println(connection.getIP());
	}
}

void loop() {

}
