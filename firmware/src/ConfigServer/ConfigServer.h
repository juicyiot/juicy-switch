#ifndef CONFIGSERVER_H
#define CONFIGSERVER_H

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include "HttpHandler.h"

class ConfigServer {
public:
	ConfigServer(const char *ssid, const char *password);
	~ConfigServer();
	void run();
	static ESP8266WebServer webServer;
	static int numAvailableNetworks;
private:
	DNSServer dnsServer;
	int scanNetworks();
};

#endif
