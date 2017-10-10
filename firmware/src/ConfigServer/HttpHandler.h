#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <ESP8266WebServer.h>

#include "ConfigServer.h"

class HttpHandler {
public:
	void handleRoot();
	void handleConfig();
	void handleConfigSave();
	void handleConfigSuccess();
	void handleConfigFailure();
	void handleNotFound();
private:
	void initializeConnection();
	bool captivePortal();
	bool isIPAddress(String str);
	String toString(IPAddress ip);
};

#endif
