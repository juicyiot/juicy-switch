#include "API.h"

API::API() {
	server = ESP8266WebServer(80);
}

API::~API() {
	server.stop();
}

void API::setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
	server.on("/on", std::bind(&API::on, this));
	server.on("/off", std::bind(&API::off, this));
	server.on("/status", std::bind(&API::status, this));
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
	if (status == 0) {
		server.send(200, "text/plain", "{'status:' 'on'}");
	} else {
		server.send(200, "text/plain", "{'status:' 'off'}");
	}
}
