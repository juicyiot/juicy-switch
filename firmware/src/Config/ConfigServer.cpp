#include "ConfigServer.h"

#include "../WiFi/WiFiConnection.h"

credentials_t ConfigServer::networkCredentials;
bool ConfigServer::shouldConnect = false;
ConnectionStatus ConfigServer::connectionStatus = none;

ConfigServer::ConfigServer(const char *ssid, const char *password, const char *hostname) {
	configNetSSID = ssid;
	configNetPassword = password;
	mdnsHostname = hostname;
	isSetup = false;
	webServer = ESP8266WebServer(PORT_WEB);
}

void ConfigServer::setup() {
	Serial.println("Config will be setup");
	// Setup access point with provided credentials
	// WiFi.softAPConfig(AP_IP, AP_IP, AP_NM);
	WiFi.softAP(configNetSSID, configNetPassword);
	delay(500);
	Serial.println(WiFi.softAPIP());

	// Setup DNS server to redirect all domains to the access point IP
	// dnsServer = DNSServer();
	// dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
	// dnsServer.start(PORT_DNS, "*", AP_IP);

	// Setup web server
	HttpHandler handler(webServer);
	webServer.on("/", std::bind(&HttpHandler::handleRoot, handler));
	webServer.on("/config", std::bind(&HttpHandler::handleConfig, handler));
	webServer.on("/configsave", std::bind(&HttpHandler::handleConfigSave, handler));
	webServer.on("/close", std::bind(&ConfigServer::handleClose, this));
	webServer.onNotFound(std::bind(&HttpHandler::handleNotFound, handler));
	webServer.begin();
	isSetup = true;
}

void ConfigServer::run() {
	if (!isSetup) {
		setup();
	}
	if (shouldConnect) {
		shouldConnect = false;
		Serial.print(networkCredentials.ssid);
		Serial.print(" ");
		Serial.println(networkCredentials.password);
		connectToNetwork();
	}
	// dnsServer.processNextRequest();
	webServer.handleClient();
}

bool ConfigServer::connectToNetwork() {
	WiFiConnection connection(networkCredentials.ssid, networkCredentials.password);

	if (connection.connect(true)) {
		connection.disconnect(false);
		connectionStatus = successful;
		// MDNS.begin(mdnsHostname);
		// MDNS.addService("http", "tcp", 80);
		return true;
	}

	connectionStatus = failed;
	return false;
}

void ConfigServer::handleClose() {
	webServer.send(200, "text/plain", "Done. You can leave now.");

	webServer.stop();
	// dnsServer.stop();
	WiFi.softAPdisconnect(true);
	delay(500);
	// WiFi.mode(WIFI_STA);
	isSetup = false;
	delay(500);
}
