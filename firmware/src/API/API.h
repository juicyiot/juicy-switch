#ifndef API_H
#define API_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

class API {
public:
	API(const char *hostname);
	~API();

	void setup();
	void serve();

private:
	std::unique_ptr<ESP8266WebServer> server;
	const char *hostname;

	void on();
	void off();
	void status();
	void notFound();
};

#endif
