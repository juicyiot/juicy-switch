#include "API.h"

#include "../WiFi/WiFiConnection.h"

const int PORT_WEB = 80;

API::API() {
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
}

API::~API() {
	server.reset();
}

void API::setup() {
	WiFi.mode(WIFI_STA);

	server.reset(new ESP8266WebServer(PORT_WEB));

	server->on("/on", std::bind(&API::on, this));
	server->on("/off", std::bind(&API::off, this));
	server->on("/status", std::bind(&API::status, this));
	server->onNotFound(std::bind(&API::notFound, this));
	server->begin();

	if (MDNS.begin("juicy_switch")) {
		MDNS.addService("http", "tcp", PORT_WEB);
	}
}

void API::serve() {
	server->handleClient();
}

void API::on() {
	digitalWrite(LED_BUILTIN, LOW);

	server->send(200, "text/plain", "{'status:' 'on'}");
}

void API::off() {
	digitalWrite(LED_BUILTIN, HIGH);

	server->send(200, "text/plain", "{'status:' 'off'}");
}

void API::status() {
	int status = digitalRead(LED_BUILTIN);

	if (status == LOW) {
		server->send(200, "text/plain", "{'status:' 'on'}");
	} else {
		server->send(200, "text/plain", "{'status:' 'off'}");
	}
}

void API::notFound() {
	server->send(200, "text/plain", "{'error:' 'not found'}");
}
