#include "ConfigServer.h"

#include "../WiFi/WiFiConnection.h"

const IPAddress AP_IP(192, 168, 4, 1);
const IPAddress AP_NM(255, 255, 255, 0);

const int PORT_WEB = 80;
const int PORT_DNS = 53;

ConfigServer::ConfigServer(const char *ssid, const char *password, const char *hostname) {
	configNetSSID = ssid;
	configNetPass = password;
	mdnsHostname = hostname;

	status = none;
	shouldConnect = false;
}

void ConfigServer::setup() {
	WiFi.mode(WIFI_AP_STA);

	webServer.reset(new ESP8266WebServer(PORT_WEB));
	dnsServer.reset(new DNSServer());

	WiFi.softAPConfig(AP_IP, AP_IP, AP_NM);
	WiFi.softAP(configNetSSID, configNetPass);

	delay(500);

	dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
	dnsServer->start(PORT_DNS, "*", WiFi.softAPIP());

	HttpHandler handler(webServer, *this);
	webServer->on("/", std::bind(&HttpHandler::handleRoot, handler));
	webServer->on("/config", std::bind(&HttpHandler::handleConfig, handler));
	webServer->on("/configsave", std::bind(&HttpHandler::handleConfigSave, handler));
	webServer->on("/close", std::bind(&HttpHandler::handleClose, handler));
	webServer->onNotFound(std::bind(&HttpHandler::handleNotFound, handler));
	webServer->begin();
}

void ConfigServer::run() {
	while (status != done) {
		if (shouldConnect) {
			shouldConnect = false;

			status = connectToNetwork() ? successful : failed;
		}

		dnsServer->processNextRequest();
		webServer->handleClient();
	}

	webServer.reset();
	dnsServer.reset();
}

bool ConfigServer::connectToNetwork() {
	WiFiConnection connection(networkCredentials.ssid, networkCredentials.password);

	if (connection.connect(true)) {
		return true;
	}

	return false;
}
