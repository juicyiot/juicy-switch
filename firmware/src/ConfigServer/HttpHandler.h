#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <ESP8266WebServer.h>

class HttpHandler {
public:
	HttpHandler(ESP8266WebServer& server, int numNetworks);
	void handleRoot();
	void handleConfig();
	void handleConfigSave();
	void handleConfigSuccess();
	void handleNotFound();
private:
	ESP8266WebServer& server;
	int numNetworks;
	bool captivePortal();
	bool isIPAddress(String str);
	String toString(IPAddress ip);
};

#endif
