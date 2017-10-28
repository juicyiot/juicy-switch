#include "Connection.h"

#include <ESP8266WiFi.h>

Connection::Connection() {
	CredentialsStorage::load(&credentials, sizeof(credentials));
	Serial.print("Loaded Credentials: ");
	Serial.print(String() + credentials.ssid + " ");
	Serial.print(String() + credentials.password + "\n");
}

Connection::Connection(const char *ssid, const char *password) {
	strncpy(credentials.ssid, ssid, CREDENTIAL_SIZE);
	strncpy(credentials.password, password, CREDENTIAL_SIZE);
}

bool Connection::connect() const {
	WiFi.mode(WIFI_STA);
	WiFi.persistent(false); // Don't automatically persist credentials. We'll do it ourselves.
	WiFi.begin(credentials.ssid, credentials.password);
	uint8_t status = WiFi.waitForConnectResult();

	return isConnected();
}

bool Connection::isConnected() const {
	wl_status_t status = WiFi.status();

	if (status != WL_CONNECTED) {
		return false;
	}

	return true;
}

void Connection::persistCredentials() const {
	CredentialsStorage::save(&credentials, sizeof(credentials));
}
