#include "ConfigServer.h"

ESP8266WebServer ConfigServer::webServer = ESP8266WebServer(PORT_WEB);
int ConfigServer::numAvailableNetworks = 0;
credentials_t ConfigServer::networkCredentials;
bool ConfigServer::shouldConnect = false;
String ConfigServer::connectionStatus = "NONE";

ConfigServer::ConfigServer(const char *ssid, const char *password) {
	// Scan available networks.
	// WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	numAvailableNetworks = WiFi.scanNetworks();

	// Setup access point with provided credentials.
	// WiFi.mode(WIFI_AP);
	WiFi.softAPConfig(AP_IP, AP_IP, AP_NM);
	WiFi.softAP(ssid, password);
	delay(500);

	// Setup DNS server to redirect all domains to the access point IP.
	dnsServer = DNSServer();
	dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
	dnsServer.start(PORT_DNS, "*", AP_IP);

	// Setup web server.
	HttpHandler handler;
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
}

void ConfigServer::run() {
	while (true) {
		if (shouldConnect) {
			shouldConnect = false;
			connectToNetwork();
		}
		dnsServer.processNextRequest();
		webServer.handleClient();
	}
}

void ConfigServer::connectToNetwork() {
	WiFi.disconnect();
	WiFi.begin(networkCredentials.ssid, networkCredentials.password);
	int res = WiFi.waitForConnectResult();
	if (res == WL_CONNECTED) {
		connectionStatus = "CONNECTION_SUCCESSFUL";
	} else {
		connectionStatus = "CONNECTION_FAILED";
	}
}
