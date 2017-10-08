#ifndef CONFIGSERVER_H
#define CONFIGSERVER_H

#include <ESP8266WebServer.h>
#include <DNSServer.h>

class ConfigServer {
public:
	ConfigServer(const char *ssid, const char *password);
	~ConfigServer();
	void run();
private:
	DNSServer dnsServer;
	ESP8266WebServer webServer;
};

#endif
