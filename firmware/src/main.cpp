#include <Arduino.h>
#include "WiFi/WiFiConnection.h"
#include "Config/ConfigServer.h"
#include "API/API.h"

const char *CONFIG_NET_SSID = "juicy_switch";
const char *CONFIG_NET_PASS = "password";
const char *API_HOST = "juicy_switch";

ConfigServer config(CONFIG_NET_SSID, CONFIG_NET_PASS);
WiFiConnection connection;
API api(API_HOST);

void setup() {
    Serial.begin(115200);
	delay(3000);

	// Try to connect using persisted credentials.
	// If the connection failes, setup a configuration AP
	// that blocks until new credentials have been provided
	// and we are successfully connected to the local network.
	connection.disconnect(true);
	if (!connection.connect()) {
		config.setup();
		config.run();
	}

	Serial.print("Connected to " + connection.getSSID() + ". Local IP: " + connection.getIP());

	api.setup();
}

void loop() {
	api.serve();
}
