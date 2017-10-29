#ifndef API_H
#define API_H

#include <Arduino.h>
#include <ESP8266WebServer.h>

class API {
public:
	API();
	~API();
	bool isSetup;
	void setup();
	void serve();
private:
	ESP8266WebServer server;
	void on();
	void off();
	void status();
	void notFound();
};

#endif
