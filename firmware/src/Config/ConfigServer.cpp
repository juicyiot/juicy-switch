#include "ConfigServer.h"

#include "../WiFi/WiFiConnection.h"

ConfigServer::ConfigServer(const char *ssid, const char *password, const char *hostname) {
	configNetSSID = ssid;
	configNetPassword = password;
	mdnsHostname = hostname;

	status = none;
	shouldConnect = false;
}

void ConfigServer::setup() {
	WiFi.mode(WIFI_AP_STA);
	webServer.reset(new ESP8266WebServer(PORT_WEB));
	WiFi.softAP(configNetSSID, configNetPassword);
	delay(500);

	// Setup web server
	HttpHandler handler(webServer, *this);
	webServer->on("/", std::bind(&HttpHandler::handleRoot, handler));
	webServer->on("/config", std::bind(&HttpHandler::handleConfig, handler));
	webServer->on("/configsave", std::bind(&HttpHandler::handleConfigSave, handler));
	webServer->on("/close", std::bind(&ConfigServer::handleClose, this));
	webServer->onNotFound(std::bind(&HttpHandler::handleNotFound, handler));
	webServer->begin();
}

void ConfigServer::runBlocking() {
	while (status != done) {
		if (shouldConnect) {
			shouldConnect = false;
			delay(500);
			Serial.println(String() + networkCredentials.ssid + " " + networkCredentials.password);
			connectToNetwork();
		}
		webServer->handleClient();
	}

	webServer.reset();
}

bool ConfigServer::connectToNetwork() {
	WiFiConnection connection(networkCredentials.ssid, networkCredentials.password);

	if (connection.connect(false)) {
		delay(500);
		status = successful;
		return true;
	}

	status = failed;
	return false;
}

void ConfigServer::handleClose() {
	webServer->send(200, "text/plain", "Done. You can leave now.");
	status = done;
}
