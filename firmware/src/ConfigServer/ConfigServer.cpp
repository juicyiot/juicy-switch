#include "ConfigServer.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "HttpHandler.h"

static const IPAddress AP_IP(192, 168, 4, 1);
static const IPAddress AP_NM(255, 255, 255, 0);

static const byte PORT_DNS = 53;
static const int PORT_WEB = 80;

ConfigServer::ConfigServer(const char *ssid, const char *password) {
	// Setup access point with provided credentials.
	WiFi.mode(WIFI_AP);
	WiFi.softAPConfig(AP_IP, AP_IP, AP_NM);
	WiFi.softAP(ssid, password);
	delay(500);

	// Setup DNS server to redirect all domains to the access point IP.
	dnsServer = DNSServer();
	dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
	dnsServer.start(PORT_DNS, "*", AP_IP);

	// Setup web server.
	webServer = ESP8266WebServer(PORT_WEB);
	HttpHandler handler(webServer);

	webServer.on("/", std::bind(&HttpHandler::handleRoot, handler));
	webServer.on("/config", std::bind(&HttpHandler::handleConfig, handler));
	webServer.on("/configsave", std::bind(&HttpHandler::handleConfigSave, handler));
	webServer.on("/configsuccess", std::bind(&HttpHandler::handleConfigSuccess, handler));
	webServer.on("/generate_204", std::bind(&HttpHandler::handleRoot, handler)); // Explicit Android captive portal
	webServer.onNotFound(std::bind(&HttpHandler::handleNotFound, handler)); // Other captive portals

	webServer.begin();
}

ConfigServer::~ConfigServer() {
	webServer.stop();
	dnsServer.stop();
	WiFi.softAPdisconnect();
}

void ConfigServer::run() {
	while (true) {
		dnsServer.processNextRequest();
		webServer.handleClient();
	}
}
