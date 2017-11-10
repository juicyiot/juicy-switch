#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <ESP8266WebServer.h>

#include "ConfigServer.h"

class HttpHandler {
public:
	HttpHandler(ESP8266WebServer &webServer);
	void handleRoot();
	void handleConfig();
	void handleConfigSave();
	void handleConfigFailure();
	void handleNotFound();
private:
	ESP8266WebServer &webServer;
	void initializeConnection();
	bool captivePortal();
	bool isIPAddress(String str);
	String toString(IPAddress ip);
};

#endif
