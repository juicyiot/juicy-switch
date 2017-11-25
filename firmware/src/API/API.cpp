#include "API.h"

#include "../WiFi/WiFiConnection.h"

const int PORT_WEB = 80;

API::API(const char *hostname) : hostname(hostname) { }

API::~API() {
	server.reset();
}

void API::addPin(uint8_t pin, uint8_t valueOn, uint8_t valueOff) {
	pins.push_back(std::make_tuple(pin, valueOn, valueOff));
}

void API::setup() {
	for (auto pin : pins) {
		pinMode(std::get<INDEX_PIN>(pin), OUTPUT);
		digitalWrite(std::get<INDEX_PIN>(pin), std::get<INDEX_VALUE_OFF>(pin));
	}

	WiFi.mode(WIFI_STA);

	server.reset(new ESP8266WebServer(PORT_WEB));

	server->on("/on", std::bind(&API::handleOn, this));
	server->on("/off", std::bind(&API::handleOff, this));
	server->on("/status", std::bind(&API::handleStatus, this));
	server->onNotFound(std::bind(&API::handleNotFound, this));
	server->begin();

	if (MDNS.begin(hostname)) {
		MDNS.addService("http", "tcp", PORT_WEB);
	}
}

void API::serve() {
	server->handleClient();
}

const char *API::status() {
	int on = 0;
	int off = 0;

	for (auto pin : pins) {
		int value = digitalRead(std::get<INDEX_PIN>(pin));
		if (value == std::get<INDEX_VALUE_ON>(pin)) {
			on++;
		} else if (value == std::get<INDEX_VALUE_OFF>(pin)) {
			off++;
		}
	}

	if (on == pins.size()) {
		return "{'status:' 'on'}";
	} else if (off == pins.size()) {
		return "{'status:' 'off'}";
	}

	return "{'status:' 'undefined'}";
}

void API::handleOn() {
	for (auto pin : pins) {
		digitalWrite(std::get<INDEX_PIN>(pin), std::get<INDEX_VALUE_ON>(pin));
	}

	server->send(200, "text/plain", status());
}

void API::handleOff() {
	for (auto pin : pins) {
		digitalWrite(std::get<INDEX_PIN>(pin), std::get<INDEX_VALUE_OFF>(pin));
	}

	server->send(200, "text/plain", status());
}

void API::handleStatus() {
	server->send(200, "text/plain", status());
}

void API::handleNotFound() {
	server->send(200, "text/plain", "{'error:' 'not found'}");
}
