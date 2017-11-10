#ifndef CONFIGSERVER_H
#define CONFIGSERVER_H

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include "HttpHandler.h"
#include "../WiFi/CredentialsStorage.h"

static const IPAddress AP_IP(192, 168, 4, 1);
static const IPAddress AP_NM(255, 255, 255, 0);

static const byte PORT_DNS = 53;
static const int PORT_WEB = 80;

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
	void runBlocking();

private:
	std::unique_ptr<ESP8266WebServer> webServer;
	const char *configNetSSID;
	const char *configNetPassword;
	const char *mdnsHostname;

	int scanNetworks();
	bool connectToNetwork();
	void handleClose();
};

#endif
