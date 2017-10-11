#include "ConfigServer.h"

ESP8266WebServer ConfigServer::webServer = ESP8266WebServer(PORT_WEB);
int ConfigServer::numAvailableNetworks = 0;
credentials_t ConfigServer::networkCredentials;
bool ConfigServer::shouldConnect = false;
bool ConfigServer::done = false;
String ConfigServer::connectionStatus = "NONE";

ConfigServer::ConfigServer(const char *configNetSSID, const char *configNetPassword) {
	// Scan available networks.
	WiFi.disconnect();
	numAvailableNetworks = WiFi.scanNetworks();

	// Try to connect using persisted credentials
	CredentialsStorage::load(&networkCredentials, sizeof(networkCredentials));
	connectToNetwork();

	// Setup access point with provided credentials.
	WiFi.softAPConfig(AP_IP, AP_IP, AP_NM);
	WiFi.softAP(configNetSSID, configNetPassword);
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
	webServer.on("/done", std::bind(&ConfigServer::handleDone, this));
	webServer.on("/generate_204", std::bind(&HttpHandler::handleRoot, handler)); // Explicit Android captive portal
	webServer.onNotFound(std::bind(&HttpHandler::handleNotFound, handler)); // Other captive portals
	webServer.begin();
}

void ConfigServer::run() {
	while (!done) {
		if (shouldConnect) {
			shouldConnect = false;
			int res = connectToNetwork();
			if (res == WL_CONNECTED) {
				connectionStatus = "CONNECTION_SUCCESSFUL";
			} else {
				connectionStatus = "CONNECTION_FAILED";
			}
		}
		dnsServer.processNextRequest();
		webServer.handleClient();
	}
	webServer.stop();
	dnsServer.stop();
	WiFi.mode(WIFI_STA);
	WiFi.softAPdisconnect(true);
}

int ConfigServer::connectToNetwork() {
	WiFi.disconnect();
	WiFi.begin(networkCredentials.ssid, networkCredentials.password);

	return WiFi.waitForConnectResult();
}

void ConfigServer::handleDone() {
	webServer.send(200, "text/plain", "Done. You can leave now.");
	done = true;
}
