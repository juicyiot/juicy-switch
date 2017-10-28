#include "ConfigServer.h"

ESP8266WebServer ConfigServer::webServer = ESP8266WebServer(PORT_WEB);
int ConfigServer::numAvailableNetworks = 0;
credentials_t ConfigServer::networkCredentials;
bool ConfigServer::shouldConnect = false;
bool ConfigServer::done = false;
ConnectionStatus ConfigServer::connectionStatus = none;

ConfigServer::ConfigServer(const char *ssid, const char *password, const char *hostname) {
	configNetSSID = ssid;
	configNetPassword = password;
	mdnsHostname = hostname;
}

void ConfigServer::setup() {
	// Scan networks
	WiFi.disconnect();
	numAvailableNetworks = WiFi.scanNetworks();

	// Setup access point with provided credentials
	WiFi.mode(WIFI_AP);
	WiFi.softAPConfig(AP_IP, AP_IP, AP_NM);
	WiFi.softAP(configNetSSID, configNetPassword);
	delay(500);

	// Setup DNS server to redirect all domains to the access point IP
	dnsServer = DNSServer();
	dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
	dnsServer.start(PORT_DNS, "*", AP_IP);

	// Setup web server
	HttpHandler handler;
	webServer.on("/", std::bind(&HttpHandler::handleRoot, handler));
	webServer.on("/config", std::bind(&HttpHandler::handleConfig, handler));
	webServer.on("/configsave", std::bind(&HttpHandler::handleConfigSave, handler));
	webServer.on("/close", std::bind(&ConfigServer::handleClose, this));
	webServer.onNotFound(std::bind(&HttpHandler::handleNotFound, handler));
	webServer.begin();
}

void ConfigServer::run() {
	if (shouldConnect) {
		shouldConnect = false;
		connectToNetwork();
	}
	dnsServer.processNextRequest();
	webServer.handleClient();
}

bool ConfigServer::connectToNetwork() {
	WiFi.disconnect();
	WiFi.begin(networkCredentials.ssid, networkCredentials.password);

	int res = WiFi.waitForConnectResult();
	if (res == WL_CONNECTED) {
		connectionStatus = successful;
		MDNS.begin(mdnsHostname);
		MDNS.addService("http", "tcp", 80);
		return true;
	}

	connectionStatus = failed;
	return false;
}

void ConfigServer::handleClose() {
	webServer.send(200, "text/plain", "Done. You can leave now.");
	done = true;

	webServer.stop();
	dnsServer.stop();
	WiFi.softAPdisconnect(true);
	WiFi.mode(WIFI_STA);
}
