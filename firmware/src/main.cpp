#include <Arduino.h>
#include "WiFi/WiFiConnection.h"
#include "Config/ConfigServer.h"
#include "API/API.h"

const char *CONFIG_NET_SSID = "juicy_switch";
const char *CONFIG_NET_PASS = "password";
const char *CONFIG_NET_HOST = "juicy";

ConfigServer config(CONFIG_NET_SSID, CONFIG_NET_PASS, CONFIG_NET_HOST);
WiFiConnection connection;
API api;

void setup() {
    Serial.begin(115200);
	delay(3000);

	// Try to connect using persisted credentials.
	// If the connection failes, setup a configuration AP
	// that blocks until new credentials have been provided
	// and we are connected to the local network.
	if (!connection.connect(false)) {
		config.setup();
		config.run();
	}

	Serial.print("Connected to " + connection.getSSID() + ". Local IP: " + connection.getIP());

	api.setup();
}

void loop() {
	api.serve();
}
