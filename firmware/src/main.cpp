#include <Arduino.h>
#include "WiFi/Connection.h"
#include "Config/ConfigServer.h"
#include <ESP8266WebServer.h>

ConfigServer config("juicy_switch", "password", "juicy");

void setup() {
    Serial.begin(115200);
	delay(5000);

	Connection connection;
	if (connection.connect()) {
		Serial.println("Connected");
		connection.persistCredentials();
	} else {
		Serial.println("Not connected");
		config.setup();
	}
}

void loop() {
	config.run();
}
