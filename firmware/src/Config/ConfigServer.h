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

enum ConnectionStatus {
	successful,
	failed,
	none
};

class ConfigServer {
public:
	ConfigServer(const char *ssid, const char *password, const char* hostname);
	void setup();
	void run();
	bool connectToNetwork();
	ESP8266WebServer webServer;
	bool isSetup;
	static ConnectionStatus connectionStatus;
	static bool shouldConnect;
	static credentials_t networkCredentials;
private:
	DNSServer dnsServer;
	int scanNetworks();
	void handleClose();
	const char *configNetSSID;
	const char *configNetPassword;
	const char *mdnsHostname;
};

#endif
