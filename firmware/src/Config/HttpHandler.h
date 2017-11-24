#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <ESP8266WebServer.h>

class ConfigServer;

class HttpHandler {
public:
	HttpHandler(std::unique_ptr<ESP8266WebServer> &webServer, ConfigServer &config);
	void handleRoot();
	void handleSetup();
	void handleSave();
	void handleStatus();
	void handleClose();
	void handleNotFound();
private:
	std::unique_ptr<ESP8266WebServer> &webServer;
	ConfigServer &config;
	void initializeConnection();
	bool captivePortal();
	bool isIPAddress(String str);
	String toString(IPAddress ip);
};

#endif
