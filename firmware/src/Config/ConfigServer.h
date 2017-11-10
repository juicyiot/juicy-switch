#ifndef CONFIGSERVER_H
#define CONFIGSERVER_H

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#include "HttpHandler.h"
#include "../WiFi/CredentialsStorage.h"

enum ConfigStatus {
	successful,
	failed,
	done,
	none
};

class ConfigServer {
public:
	ConfigServer(const char *ssid, const char *password, const char* hostname);

	ConfigStatus status;
	bool shouldConnect;
	credentials_t networkCredentials;

	void setup();
	void run();

private:
	std::unique_ptr<ESP8266WebServer> webServer;
	
	const char *configNetSSID;
	const char *configNetPass;
	const char *mdnsHostname;

	int scanNetworks();
	bool connectToNetwork();
	void handleClose();
};

#endif
