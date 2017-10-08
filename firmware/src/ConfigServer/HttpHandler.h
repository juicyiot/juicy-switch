#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <ESP8266WebServer.h>

class HttpHandler {
public:
	HttpHandler(ESP8266WebServer& server);
	void handleRoot();
	void handleNotFound();
private:
	ESP8266WebServer& server;
	bool captivePortal();
	bool isIPAddress(String str);
	String toString(IPAddress ip);
};

#endif
