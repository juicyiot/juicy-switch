#ifndef API_H
#define API_H

#include <vector>
#include <tuple>

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const int INDEX_PIN = 0;
const int INDEX_VALUE_ON = 1;
const int INDEX_VALUE_OFF = 2;

class API {
public:
	API(const char *hostname);
	~API();

	void setup();
	void serve();

	void addPin(uint8_t pin, uint8_t valueOn, uint8_t valueOff);

private:
	std::unique_ptr<ESP8266WebServer> server;
	const char *hostname;

	std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> pins;

	const char *status();

	void handleOn();
	void handleOff();
	void handleStatus();
	void handleNotFound();
};

#endif
