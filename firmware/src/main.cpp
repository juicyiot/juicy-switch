#include <Arduino.h>
#include "ConfigServer/ConfigServer.h"
#include <ESP8266WebServer.h>

#include <vector>
#include <string>

ESP8266WebServer server(80);

void setup() {
    Serial.begin(115200);
	delay(3000);

	ConfigServer config("juicy_switch", "password");
	config.run();
}

void loop() {
    // put your main code here, to run repeatedly:
}
