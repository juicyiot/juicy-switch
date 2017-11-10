#include "API.h"

#include "../WiFi/WiFiConnection.h"

API::API() {
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
}

API::~API() {
	server.stop();
}

void API::setup() {
	Serial.println("Setting up API.");
	
	WiFi.mode(WIFI_STA);
	server = ESP8266WebServer(80);
	server.on("/on", std::bind(&API::on, this));
	server.on("/off", std::bind(&API::off, this));
	server.on("/status", std::bind(&API::status, this));
	server.onNotFound(std::bind(&API::notFound, this));

	server.begin();
}

void API::serve() {
	server.handleClient();
}

void API::on() {
	digitalWrite(LED_BUILTIN, LOW);

	server.send(200, "text/plain", "{'status:' 'on'}");
}

void API::off() {
	digitalWrite(LED_BUILTIN, HIGH);

	server.send(200, "text/plain", "{'status:' 'off'}");
}

void API::status() {
	int status = digitalRead(LED_BUILTIN);

	if (status == LOW) {
		server.send(200, "text/plain", "{'status:' 'on'}");
	} else {
		server.send(200, "text/plain", "{'status:' 'off'}");
	}
}

void API::notFound() {
	server.send(200, "text/plain", "{'error:' 'not found'}");
}
