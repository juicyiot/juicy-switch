#include "ConfigServer.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

static const IPAddress AP_IP(192, 168, 4, 1);
static const IPAddress AP_NM(255, 255, 255, 0);

static const byte PORT_DNS = 53;
static const int PORT_WEB = 80;

ConfigServer::ConfigServer(const char *ssid, const char *password) {
	// Setup access point.
	WiFi.mode(WIFI_AP);
	WiFi.softAPConfig(AP_IP, AP_IP, AP_NM);
	WiFi.softAP(ssid, password);
	delay(500);
	Serial.println(WiFi.softAPIP());

	// Setup DNS server to redirect all domains to the access point IP.
	dnsServer = DNSServer();
	dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
	dnsServer.start(PORT_DNS, "*", AP_IP);

	// Setup web server.
	webServer = ESP8266WebServer(PORT_WEB);
	webServer.on("/", HTTP_GET, [this]() { handleRoot(); });
	webServer.begin();
}

ConfigServer::~ConfigServer() {
	webServer.stop();
	dnsServer.stop();
	WiFi.softAPdisconnect(true);
}

void ConfigServer::run() {
	while (true) {
		dnsServer.processNextRequest();
		webServer.handleClient();
	}
}

void ConfigServer::handleRoot() {
	Serial.println("Root!");
	webServer.send(200, "text/plain", "juicy config");
}
