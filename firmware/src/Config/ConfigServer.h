#ifndef CONFIGSERVER_H
#define CONFIGSERVER_H

#include <functional>

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>

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
	ConfigServer(const char *ssid, const char *password);

	ConfigStatus status;
	bool shouldConnect;
	credentials_t networkCredentials;

	std::function<void()> connectionSuccessCallback;
	std::function<void()> connectionFailureCallback;

	void setup();
	void run();

private:
	std::unique_ptr<ESP8266WebServer> webServer;
	std::unique_ptr<DNSServer> dnsServer;

	const char *configNetSSID;
	const char *configNetPass;
	const char *mdnsHostname;

	int scanNetworks();
	bool connectToNetwork();
	void handleClose();
};

#endif
