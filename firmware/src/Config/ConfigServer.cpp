#include "ConfigServer.h"

#include "../WiFi/WiFiConnection.h"

const IPAddress AP_IP(192, 168, 4, 1);
const IPAddress AP_NM(255, 255, 255, 0);

const int PORT_WEB = 80;
const int PORT_DNS = 53;

ConfigServer::ConfigServer(const char *ssid, const char *password) {
	configNetSSID = ssid;
	configNetPass = password;

	status = none;
	shouldConnect = false;

	connectionSuccessCallback = []() {
		pinMode(LED_BUILTIN, OUTPUT);
		digitalWrite(LED_BUILTIN, HIGH);

		digitalWrite(LED_BUILTIN, LOW);
		delay(2000);
		digitalWrite(LED_BUILTIN, HIGH);
	};

	connectionFailureCallback = []() {
		pinMode(LED_BUILTIN, OUTPUT);
		digitalWrite(LED_BUILTIN, HIGH);

		for (int i = 0; i < 2; i++) {
			digitalWrite(LED_BUILTIN, LOW);
			delay(500);
			digitalWrite(LED_BUILTIN, HIGH);
			delay(500);
		}
	};
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
	webServer->on("/setup", std::bind(&HttpHandler::handleSetup, handler));
	webServer->on("/save", std::bind(&HttpHandler::handleSave, handler));
	webServer->on("/status", std::bind(&HttpHandler::handleStatus, handler));
	webServer->on("/close", std::bind(&HttpHandler::handleClose, handler));
	webServer->onNotFound(std::bind(&HttpHandler::handleNotFound, handler));
	webServer->begin();
}

void ConfigServer::run() {
	while (status != done) {
		dnsServer->processNextRequest();
		webServer->handleClient();

		if (shouldConnect) {
			shouldConnect = false;

			status = connectToNetwork() ? successful : failed;

			if (status == successful) {
				connectionSuccessCallback();
			} else {
				connectionFailureCallback();
			}
		}
	}

	webServer.reset();
	dnsServer.reset();
}

bool ConfigServer::connectToNetwork() {
	WiFiConnection connection(networkCredentials.ssid, networkCredentials.password);

	if (connection.connect()) {
		connection.persistCredentials();

		return true;
	}

	return false;
}
